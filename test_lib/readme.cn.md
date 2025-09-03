这里是为了测试链接库的修改是否会影响到可执行文件的值执行

# 1 build
build bin and lib
lib not defind function declaration in h file and bin not use
build OK
bin run OK

# 2 level1 add function
bin old
lib new, add function
bin run OK

# 3 level1 del function
bin old
lib new, del function bin not use
bin run OK

bin old
lib new, del function bin use
bin run undefined symbol

# 4 level2 add struct field
bin old
lib new, add field
bin run OK
but new field show rand value, mem overread

# 5 level2 del struct field
bin old
lib new, del field bin not use
bin run OK

bin old
lib new, del field bin use
bin run OK

bin old
lib new, del field mid
run OK, data dislocation
bin or lib use self struct


