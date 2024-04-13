compiler=gcc
src=process.c main.c
target=process

$(target):$(src)
	$(compiler) $^ -o $@
.PHONY:clean
clean:
	rm -f $(target)

