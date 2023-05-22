#!/bin/bash


if [ $# -gt 0 ]
then
	MODE=$1
else
	MODE=default
fi

NLCB=$MODE ./venv/bin/python3 main.py
