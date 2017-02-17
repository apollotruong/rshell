#!/bin/sh

echo test;ls && mkdir lol;ls;rmdir lol; ls || echo this should not output
