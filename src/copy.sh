#!/bin/bash

scp /home/gitlab-runner/builds/CSbqKAvX/0/students/DO6_CICD.ID_356283/zenaluth_student.21_school.ru/DO6_CICD-1/src/cat/s21_cat zen@10.10.0.2:/usr/local/bin/
scp /home/gitlab-runner/builds/CSbqKAvX/0/students/DO6_CICD.ID_356283/zenaluth_student.21_school.ru/DO6_CICD-1/src/grep/s21_grep zen@10.10.0.2:/usr/local/bin/
ssh zen@10.10.0.2 ls -lah /usr/local/bin