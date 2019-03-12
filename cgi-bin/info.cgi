{\rtf1\ansi\ansicpg1252\cocoartf1671\cocoasubrtf200
{\fonttbl\f0\fnil\fcharset0 Menlo-Regular;}
{\colortbl;\red255\green255\blue255;\red0\green0\blue0;\red244\green244\blue235;}
{\*\expandedcolortbl;;\cssrgb\c0\c0\c0;\cssrgb\c96471\c96471\c93725;}
\margl1440\margr1440\vieww10800\viewh8400\viewkind0
\deftab720
\pard\pardeftab720\sl300\partightenfactor0

\f0\fs26 \cf2 \expnd0\expndtw0\kerning0
#!/usr/bin/perl\
use strict;\
use warnings;\
use CGI;\
my $cgi = CGI->new();\
print $cgi->header(-type => \'91image/jpg\'92);\
open my $img, \'91<\'91, \'91../images/fortniteVSapex.jpg\'92;\
while (<$img>) \{ print \}\
}