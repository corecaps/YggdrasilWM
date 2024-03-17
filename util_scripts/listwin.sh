#!/bin/bash
for id in $(xdotool search --name ".*"); do
  name=$(xdotool getwindowname $id)
  echo "Window ID: $id - '$name'"
done
