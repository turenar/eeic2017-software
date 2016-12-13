#!/usr/bin/python3
# encoding: utf-8
import os
import re
import tempfile
import argparse

import sys


class SourceCombiner(object):
    _pragma_once_regex = re.compile(r'\s*#pragma\s+once')
    _local_include_regex = re.compile(r'^\s*#include\s+"(.+)"\s*$')
    _system_include_regex = re.compile(r'^\s*#include\s+<(.+)>\s*$')

    def __init__(self, out_file):
        self.out_file_name = out_file
        self.tmp_file = tempfile.TemporaryFile("w+t")
        self.included = set()
        self.system_includes = set()

    def _parse_line(self, filename, line, level):
        pragma_once_match = self._pragma_once_regex.match(line)
        if pragma_once_match:
            return

        local_include_match = self._local_include_regex.match(line)
        if local_include_match:
            include_file = local_include_match.group(1)
            if include_file not in self.included:
                sys.stderr.write('{} include: {}\n'.format('>' * level, include_file))
                self.tmp_file.write("/* start: {} */\n".format(include_file))
                self._include(include_file, level + 1)
                self.tmp_file.write("/* end: {} */\n".format(include_file))
            return

        system_include_match = self._system_include_regex.match(line)
        if system_include_match:
            include_file = system_include_match.group(1)
            if include_file not in self.system_includes:
                # sys.stderr.write('{} sys-include: {}\n'.format('>' * level, include_file))
                self.tmp_file.write("#include <{}>\n".format(include_file))
                # self.system_includes.add(include_file)
            return

        self.tmp_file.write(line)

    def _include(self, filename, level=1):
        self.included.add(filename)
        with open(filename, "r") as f:
            for line in f:
                # print(line)
                self._parse_line(filename, line, level)

    def run(self, in_file):
        self._include(in_file)
        self.tmp_file.seek(0)
        with open(self.out_file_name, "w") as out_file:
            out_file.write('/* This file is auto-generated */\n')
            # for sys_include in sorted(self.system_includes):
            #     out_file.write('#include <{}>\n'.format(sys_include))
            for line in self.tmp_file:
                out_file.write(line)
        with open(self.out_file_name + '.d', "w") as dep_file:
            dep_file.write('{}: {} {}\n'.format(self.out_file_name, in_file, ' '.join(self.included)))


def main(base_file, out_file):
    sc = SourceCombiner(out_file)
    sc.run(base_file)


if __name__ == '__main__':
    if len(sys.argv) == 4:
        __option = sys.argv[1]
        __in_file = sys.argv[2]
        __out_file = sys.argv[3]
    else:
        __option = None
        __in_file = sys.argv[1]
        __out_file = sys.argv[2]
    overwrite_confirm = True
    if __option:
        if __option == '-f':
            overwrite_confirm = False
        else:
            sys.stderr.write('unknown option: {}'.format(__option))
            sys.exit(1)
    if overwrite_confirm and os.path.exists(__out_file):
        sys.stderr.write('{} exists. overwrite? [y/n]> '.format(__out_file))
        sys.stderr.flush()
        while True:
            ans = sys.stdin.readline().rstrip()
            if ans == 'y' or ans == 'yes':
                break
            elif ans == 'n' or ans == 'no':
                sys.exit(0)
            else:
                sys.stderr.write('answer y or n > ')
                sys.stderr.flush()
    main(__in_file, __out_file)
