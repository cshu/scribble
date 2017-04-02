
//why a file system layer is needed?
//a file (maybe a short note) is a noun in the ddb
//when you rename/move a file, the path to access the file is changed. if ddb doesn't know that, it causes confusion.
//so when you rename/move/write/delete a file, it must be done through this layer.

//you also need a simple utility tool to periodically check if any file is inconsistent with registration information. (it's always possible that some files are changed by someone/something)

//when ddb can not access a directory of registered files, the status is recognized as "currently unreachable" (e.g. external drive)

//also you may consider document indexing

