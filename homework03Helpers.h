// Copyright 2024 Alan M. Ferrenberg
#ifndef HOMEWORK03HELPERS_H_
#define HOMEWORK03HELPERS_H_

// This file contains some helper functions for the homework03 assignment
// Please feel free to study these, as they provide some interesting
// functionality, but do not modify them!!!!!!!!!

// #############################################################################
// ################ DO NOT MODIFY -- YOU HAVE BEEN WARNED ######################
// #############################################################################
// ################ DO NOT MODIFY -- YOU HAVE BEEN WARNED ######################
// #############################################################################
// ################ DO NOT MODIFY -- YOU HAVE BEEN WARNED ######################
// #############################################################################
// ################ DO NOT MODIFY -- YOU HAVE BEEN WARNED ######################
// #############################################################################
// ################ DO NOT MODIFY -- YOU HAVE BEEN WARNED ######################
// #############################################################################
// ################ DO NOT MODIFY -- YOU HAVE BEEN WARNED ######################
// #############################################################################
// ################ DO NOT MODIFY -- YOU HAVE BEEN WARNED ######################
// #############################################################################
// ################ DO NOT MODIFY -- YOU HAVE BEEN WARNED ######################
// #############################################################################
#include <dirent.h>
#include <unistd.h>
#include <memory>
#include <stdexcept>
#include <array>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdio>
#include <string>
#include <cstring>
#include <cstdint>

// Structure that contains all of the fields in the /proc/[pid]/stat file
struct procStat {
    int32_t  pid;
    std::string tcomm;
    char state;
    int32_t ppid, pgrp, sid, tty_nr, tty_pgrp;
    uint32_t flags, min_flt, cmin_flt, maj_flt, cmaj_flt, utime, stime;
    int32_t cutime, cstime, priority, nice, num_threads;
    uint32_t it_real_value, start_time, vsize, rss, rsslim, start_code;
    uint32_t end_code, start_stack, esp, eip, pending, blocked, sigign;
    uint32_t sigcatch, wchan, dum1, dum2;
    int32_t exit_signal, task_cpu;
    uint32_t rt_priority, policy, blkio_ticks, gtime;
    int32_t cgtime;
    uint32_t start_data, end_data, start_brk, arg_start, arg_end, env_start;
    uint32_t env_end;
    int32_t exit_code;
};

// Populate a procStat structure with the contents of a /proc/[pid]/stat file
procStat parseStatFile(const std::string pid) {
    // procStat variable to hold the return value
    procStat result;
    // Open the /proc/[pid]stat file as a stream
    std::string filename = "/proc/" + pid + "/stat";
    std::ifstream input(filename);
    // Read the stream and populate the individual fields of result
    input >> result.pid >> result.tcomm >> result.state >> result.ppid
          >> result.pgrp >> result.sid >> result.tty_nr >> result.tty_pgrp
          >> result.flags >> result.min_flt >> result.cmin_flt
          >> result.maj_flt >> result.cmaj_flt >> result.utime >> result.stime
          >> result.cutime >> result.cstime >> result.priority
          >> result.nice >> result.num_threads >> result.it_real_value
          >> result.start_time >> result.vsize >> result.rss >> result.rsslim
          >> result.start_code >> result.end_code >> result.start_stack
          >> result.esp >> result.eip >> result.pending >> result.blocked
          >> result.sigign >> result.sigcatch >> result.wchan >> result.dum1
          >> result.dum2 >> result.exit_signal >> result.task_cpu
          >> result.rt_priority >> result.policy >> result.blkio_ticks
          >> result.gtime >> result.cgtime >> result.start_data
          >> result.end_data >> result.start_brk >> result.arg_start
          >> result.arg_end >> result.env_start >> result.env_end
          >> result.exit_code;

    return result;
}

// Helper function to check if a string is numeric
bool isNumeric(const std::string x) {
    for (const char &character : x) {
        if (!isdigit(character)) return false;
    }
    return true;
}

// Files in /proc that are associated with running processes have names that are
// numeric (the pid of the process).  Populate a vector of strings with the pids
// that are numeric.
void getProcFiles(std::vector<std::string>& procFiles) {
    // Clear out the procFiles vector so we're starting clean
    procFiles.clear();
    // Create a pointer to the directory
    DIR *directoryPointer;
    directoryPointer = opendir("/proc");

    // Loop through all of the files in the directory and add them to a
    // string vector
    for (struct dirent *dent; (dent = readdir(directoryPointer)) != NULL; ) {
        std::string nameToCheck = dent->d_name;
        if (isNumeric(nameToCheck)) {
            procFiles.push_back(nameToCheck);
        }
    }
}
#endif  // HOMEWORK03HELPERS_H_
