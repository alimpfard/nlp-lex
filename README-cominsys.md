# Steps to use

## Step 0 - build compiler

Refer to main readme

## Step 1 - compile source to library

`build/nlex --output tokeniser.obj --library --target-sys windows --object-format coff --target-arch x86_64 ../examples/test.nlex`

## Step 2.1 - build a dll

`link /dll /def:tokeniser.def tokeniser.obj`

## Step 2.2 - compile against the C wrapper

`cd ../wrapper/c`
`cc wrapper.c ../src/tokeniser.obj`

And use `a.out`


### C wrapper options format

```
./a.out [options] -- input_files...

[OPTIONS]
    --json : Enable json output
    --output [output_file] : write json to given file
```
