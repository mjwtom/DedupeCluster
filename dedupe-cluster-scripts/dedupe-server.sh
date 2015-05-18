#!/bin/bash
workspace=/home/mjwtom/workspace

$workspace/dedupe-server/Debug/dedupe-server /home/mjwtom/dedupe-file1 6668 &
$workspace/dedupe-server/Debug/dedupe-server /home/mjwtom/dedupe-file2 6669 &
