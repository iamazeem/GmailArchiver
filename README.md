# GmailArchiver

Gmail Archiver - a console utility to archive email messages in encrypted format

## Build Instructions

Prerequisites:
- CppRestSdk
- Boost::System
- CMake

```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Usage

```
./GmailArchiver <filename> <start-date> <end-date>
```

* `<filename>`   - output filename to save encrypted email messages
* `<start-date>` - starting date of email messages to be archived
* `<end-date>`   - ending date of email messages to be archived

**NOTE**: Starting and Ending dates should be in the format [YYYY/MM/DD].
