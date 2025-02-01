#!/usr/bin/sh

DIR=../build

execute_command() {
  for cmd in "$@"; do
    echo "Executing: $cmd"
    eval "$cmd"
  done
}

check_if_test_lib_exist() {
  if [ ! -d $DIR ]; then
    execute_command "mkdir -p $DIR" "cd $DIR" "cmake .."
  elif [ -z "$(ls -A "$DIR")" ]; then
      execute_command "cd $DIR" "cmake .."
  fi
  echo "google test lib ok..."
}

build_test_lib() {
  execute_command "cd .." "cmake --build build -j 12"
}

check_if_test_lib_exist
build_test_lib