#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>
int64_t sync_log_num_, rsync_log_num_, binlog_log_num_;

int test(uint64_t number) {
    int keep = 0;
    if (sync_log_num_ != -1 && number >= (uint64_t)sync_log_num_) {
      keep = 1;
    }   
    if (rsync_log_num_ != -1 && number >= (uint64_t)rsync_log_num_) {
      keep = 1;
    }   
    if (binlog_log_num_ != -1 && number >= (uint64_t)binlog_log_num_) {
      keep = 1;
    }
    return keep;
}
int main() {
    //全部没有开启 
    sync_log_num_ = 0, rsync_log_num_ = -1, binlog_log_num_ = -1;
    assert(test(1) == 1);
    //binlog没有开启，跨机房没有开启, 只有主从
    sync_log_num_ = 100, rsync_log_num_ = -1, binlog_log_num_ = -1;
    assert(test(20) == 0); 
    assert(test(200) == 1); 
    //binlog开启，跨机房没有开启, 主从开启, binlog慢主从
    sync_log_num_ = 100, rsync_log_num_ = -1, binlog_log_num_ = 10;
    assert(test(20) == 1); 
    assert(test(200) == 1); 
    //binlog开启，跨机房没有开启, 主从开启, binlog快主从
    sync_log_num_ = 100, rsync_log_num_ = -1, binlog_log_num_ = 1000;
    assert(test(20) == 0); 
    assert(test(200) == 1); 
    //binlog开启，跨机房没有开启, 主从开启, binlog和主从一样快
    sync_log_num_ = 100, rsync_log_num_ = -1, binlog_log_num_ = 100;
    assert(test(20) == 0); 
    assert(test(100) == 1); 
    //binlog没有开启，跨机房开启, 主从开启, 跨机房和主从一样快
    sync_log_num_ = 100, rsync_log_num_ = 100, binlog_log_num_ = -1;
    assert(test(20) == 0); 
    assert(test(100) == 1); 
    //binlog没有开启，跨机房开启, 主从开启, 跨机房慢主从
    sync_log_num_ = 100, rsync_log_num_ = 10, binlog_log_num_ = -1;
    assert(test(20) == 1); 
    assert(test(1) == 0); 
    //binlog没有开启，跨机房开启, 主从开启, 跨机房快主从
    sync_log_num_ = 100, rsync_log_num_ = 1000, binlog_log_num_ = -1;
    assert(test(20) == 0); 
    assert(test(150) == 1); 
    //binlog开启，跨机房开启, 主从开启, 主从最快，跨机房第2，binlog最慢
    sync_log_num_ = 100, rsync_log_num_ = 50, binlog_log_num_ = 10;
    assert(test(2) == 0); 
    assert(test(20) == 1); 
    assert(test(70) == 1); 
    assert(test(150) == 1); 
    //binlog开启，跨机房开启, 主从开启, 主从最快，跨机房最慢
    sync_log_num_ = 100, rsync_log_num_ = 10, binlog_log_num_ = 50;
    assert(test(2) == 0); 
    assert(test(20) == 1); 
    assert(test(70) == 1); 
    assert(test(150) == 1); 
         
    return 0;
}
