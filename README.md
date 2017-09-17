# LSHeader

Travis CI build status: [![Build Status](https://travis-ci.org/LBYPatrick/LSHeader.svg)](https://travis-ci.org/LBYPatrick/LSHeader)
<br>

Want to add header of source code file for Mr.Patel without using the disgusting eclipse? Here's a great way to do so. 

And it is designed to be **cross-platform**!

## Why do I need this?

Everyone knows that Eclipse is the most disgusting IDE in the universe.

## Usage

You can write a configuration file named ``lsheader.ini`` for saving efforts in commandline like this:
```
name = LBYPatrick
period = 2
date = Payday, 1984
Project Title = oh I love Java
Project Description = Explaining benifits of using Java in a religious approach.
Version = 1.1000.1
```
The configuration file is **NOT REQUIRED**, and you **don't even need** to specify ``version`` or ``date`` in both commandline and in the config file since they could be auto-generated.


And use the commandline, contents will be overwritten if you specify something that was existed in the config:
```bash
		-i or --input				specify input file
		-o or --output				specify output file
		-a or --author				specify author ("Name")
		-d or --date				specify date (optional)
		-p or --period				specify period
       -pt or --project-title		specify project Title
	   -pd or --project-description specify project description
	    -v or --version				specify version

```

## Upcoming features

- Custom config file name
- GUI interface (WPF)