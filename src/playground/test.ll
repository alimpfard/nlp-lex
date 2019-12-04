; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.sresult = type { i8*, i32, i8*, i8, i8, i8*, i32 }
%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i64, i16, i8, [1 x i8], i8*, i64, %struct._IO_codecvt*, %struct._IO_wide_data*, %struct._IO_FILE*, i8*, i64, i32, [20 x i8] }
%struct._IO_marker = type opaque
%struct._IO_codecvt = type opaque
%struct._IO_wide_data = type opaque
%struct.__va_list_tag = type { i32, i32, i8*, i8* }

@res = global %struct.sresult zeroinitializer, align 8, !dbg !0
@stderr = external global %struct._IO_FILE*, align 8
@.str = private unnamed_addr constant [3 x i8] c"%c\00", align 1
@.str.1 = private unnamed_addr constant [4 x i8] c"%lf\00", align 1
@metadata = internal global i32 0, align 4, !dbg !19
@.str.2 = private unnamed_addr constant [20 x i8] c"res at %p, s at %p\0A\00", align 1
@.str.3 = private unnamed_addr constant [3 x i8] c"> \00", align 1
@stdin = external global %struct._IO_FILE*, align 8
@.str.4 = private unnamed_addr constant [23 x i8] c"Error from read(2): %s\00", align 1
@.str.5 = private unnamed_addr constant [19 x i8] c"processing - '%s'\0A\00", align 1
@.str.6 = private unnamed_addr constant [46 x i8] c"%smatch {'%.*s' - %s - %d %s} is%sa stopword\0A\00", align 1
@.str.7 = private unnamed_addr constant [4 x i8] c"no \00", align 1
@.str.8 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@.str.9 = private unnamed_addr constant [2 x i8] c" \00", align 1
@.str.10 = private unnamed_addr constant [6 x i8] c" not \00", align 1

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define double @putchard(double) #0 !dbg !38 {
  %2 = alloca double, align 8
  %3 = alloca i8, align 1
  %4 = alloca i32, align 4
  store double %0, double* %2, align 8
  call void @llvm.dbg.declare(metadata double* %2, metadata !41, metadata !DIExpression()), !dbg !42
  call void @llvm.dbg.declare(metadata i8* %3, metadata !43, metadata !DIExpression()), !dbg !44
  %5 = load double, double* %2, align 8, !dbg !45
  %6 = fptosi double %5 to i8, !dbg !46
  store i8 %6, i8* %3, align 1, !dbg !44
  call void @llvm.dbg.declare(metadata i32* %4, metadata !47, metadata !DIExpression()), !dbg !48
  %7 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 8, !dbg !49
  %8 = load i8, i8* %3, align 1, !dbg !50
  %9 = sext i8 %8 to i32, !dbg !50
  %10 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %7, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i64 0, i64 0), i32 %9), !dbg !51
  store i32 %10, i32* %4, align 4, !dbg !48
  %11 = load i32, i32* %4, align 4, !dbg !52
  %12 = sitofp i32 %11 to double, !dbg !53
  ret double %12, !dbg !54
}

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

declare i32 @fprintf(%struct._IO_FILE*, i8*, ...) #2

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define double @eputchard(double) #0 !dbg !55 {
  %2 = alloca double, align 8
  %3 = alloca i8, align 1
  %4 = alloca i32, align 4
  store double %0, double* %2, align 8
  call void @llvm.dbg.declare(metadata double* %2, metadata !56, metadata !DIExpression()), !dbg !57
  call void @llvm.dbg.declare(metadata i8* %3, metadata !58, metadata !DIExpression()), !dbg !59
  %5 = load double, double* %2, align 8, !dbg !60
  %6 = fptosi double %5 to i8, !dbg !61
  store i8 %6, i8* %3, align 1, !dbg !59
  call void @llvm.dbg.declare(metadata i32* %4, metadata !62, metadata !DIExpression()), !dbg !63
  %7 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 8, !dbg !64
  %8 = load i8, i8* %3, align 1, !dbg !65
  %9 = sext i8 %8 to i32, !dbg !65
  %10 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %7, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i64 0, i64 0), i32 %9), !dbg !66
  store i32 %10, i32* %4, align 4, !dbg !63
  %11 = load i32, i32* %4, align 4, !dbg !67
  %12 = sitofp i32 %11 to double, !dbg !68
  ret double %12, !dbg !69
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define double @printd(double) #0 !dbg !70 {
  %2 = alloca double, align 8
  store double %0, double* %2, align 8
  call void @llvm.dbg.declare(metadata double* %2, metadata !71, metadata !DIExpression()), !dbg !72
  %3 = load double, double* %2, align 8, !dbg !73
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i64 0, i64 0), double %3), !dbg !74
  %5 = sitofp i32 %4 to double, !dbg !75
  ret double %5, !dbg !76
}

declare i32 @printf(i8*, ...) #2

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define double @eprintd(double) #0 !dbg !77 {
  %2 = alloca double, align 8
  store double %0, double* %2, align 8
  call void @llvm.dbg.declare(metadata double* %2, metadata !78, metadata !DIExpression()), !dbg !79
  %3 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 8, !dbg !80
  %4 = load double, double* %2, align 8, !dbg !81
  %5 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i64 0, i64 0), double %4), !dbg !82
  %6 = sitofp i32 %5 to double, !dbg !83
  ret double %6, !dbg !84
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define double @dprintdf(double, double, ...) #0 !dbg !85 {
  %3 = alloca double, align 8
  %4 = alloca double, align 8
  %5 = alloca [1 x %struct.__va_list_tag], align 16
  %6 = alloca i64, align 8
  %7 = alloca double, align 8
  store double %0, double* %3, align 8
  call void @llvm.dbg.declare(metadata double* %3, metadata !88, metadata !DIExpression()), !dbg !89
  store double %1, double* %4, align 8
  call void @llvm.dbg.declare(metadata double* %4, metadata !90, metadata !DIExpression()), !dbg !91
  call void @llvm.dbg.declare(metadata [1 x %struct.__va_list_tag]* %5, metadata !92, metadata !DIExpression()), !dbg !108
  %8 = getelementptr inbounds [1 x %struct.__va_list_tag], [1 x %struct.__va_list_tag]* %5, i64 0, i64 0, !dbg !109
  %9 = bitcast %struct.__va_list_tag* %8 to i8*, !dbg !109
  call void @llvm.va_start(i8* %9), !dbg !109
  call void @llvm.dbg.declare(metadata i64* %6, metadata !110, metadata !DIExpression()), !dbg !111
  %10 = load double, double* %4, align 8, !dbg !112
  %11 = fptosi double %10 to i64, !dbg !112
  store i64 %11, i64* %6, align 8, !dbg !111
  call void @llvm.dbg.declare(metadata double* %7, metadata !113, metadata !DIExpression()), !dbg !114
  %12 = load double, double* %3, align 8, !dbg !115
  %13 = fptosi double %12 to i32, !dbg !116
  %14 = load i64, i64* %6, align 8, !dbg !117
  %15 = inttoptr i64 %14 to i8*, !dbg !118
  %16 = getelementptr inbounds [1 x %struct.__va_list_tag], [1 x %struct.__va_list_tag]* %5, i64 0, i64 0, !dbg !119
  %17 = call i32 @vdprintf(i32 %13, i8* %15, %struct.__va_list_tag* %16), !dbg !120
  %18 = sitofp i32 %17 to double, !dbg !121
  store double %18, double* %7, align 8, !dbg !114
  %19 = getelementptr inbounds [1 x %struct.__va_list_tag], [1 x %struct.__va_list_tag]* %5, i64 0, i64 0, !dbg !122
  %20 = bitcast %struct.__va_list_tag* %19 to i8*, !dbg !122
  call void @llvm.va_end(i8* %20), !dbg !122
  %21 = load double, double* %7, align 8, !dbg !123
  ret double %21, !dbg !124
}

; Function Attrs: nounwind
declare void @llvm.va_start(i8*) #3

declare i32 @vdprintf(i32, i8*, %struct.__va_list_tag*) #2

; Function Attrs: nounwind
declare void @llvm.va_end(i8*) #3

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define double @mallocd(double) #0 !dbg !125 {
  %2 = alloca double, align 8
  %3 = alloca i64, align 8
  %4 = alloca i8*, align 8
  %5 = alloca i64, align 8
  store double %0, double* %2, align 8
  call void @llvm.dbg.declare(metadata double* %2, metadata !126, metadata !DIExpression()), !dbg !127
  call void @llvm.dbg.declare(metadata i64* %3, metadata !128, metadata !DIExpression()), !dbg !129
  %6 = load double, double* %2, align 8, !dbg !130
  %7 = fptoui double %6 to i64, !dbg !131
  store i64 %7, i64* %3, align 8, !dbg !129
  call void @llvm.dbg.declare(metadata i8** %4, metadata !132, metadata !DIExpression()), !dbg !133
  %8 = load i64, i64* %3, align 8, !dbg !134
  %9 = call noalias i8* @malloc(i64 %8) #3, !dbg !135
  store i8* %9, i8** %4, align 8, !dbg !133
  call void @llvm.dbg.declare(metadata i64* %5, metadata !136, metadata !DIExpression()), !dbg !137
  %10 = load i8*, i8** %4, align 8, !dbg !138
  %11 = ptrtoint i8* %10 to i64, !dbg !139
  store i64 %11, i64* %5, align 8, !dbg !137
  %12 = load i64, i64* %5, align 8, !dbg !140
  %13 = sitofp i64 %12 to double, !dbg !141
  ret double %13, !dbg !142
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #4

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define double @memsetd(double, double, double) #0 !dbg !143 {
  %4 = alloca double, align 8
  %5 = alloca double, align 8
  %6 = alloca double, align 8
  %7 = alloca i64, align 8
  store double %0, double* %4, align 8
  call void @llvm.dbg.declare(metadata double* %4, metadata !146, metadata !DIExpression()), !dbg !147
  store double %1, double* %5, align 8
  call void @llvm.dbg.declare(metadata double* %5, metadata !148, metadata !DIExpression()), !dbg !149
  store double %2, double* %6, align 8
  call void @llvm.dbg.declare(metadata double* %6, metadata !150, metadata !DIExpression()), !dbg !151
  call void @llvm.dbg.declare(metadata i64* %7, metadata !152, metadata !DIExpression()), !dbg !153
  %8 = load double, double* %4, align 8, !dbg !154
  %9 = fptosi double %8 to i64, !dbg !154
  store i64 %9, i64* %7, align 8, !dbg !153
  %10 = load i64, i64* %7, align 8, !dbg !155
  %11 = inttoptr i64 %10 to i8*, !dbg !156
  %12 = load double, double* %5, align 8, !dbg !157
  %13 = fptosi double %12 to i32, !dbg !158
  %14 = trunc i32 %13 to i8, !dbg !159
  %15 = load double, double* %6, align 8, !dbg !160
  %16 = fptoui double %15 to i64, !dbg !161
  call void @llvm.memset.p0i8.i64(i8* align 1 %11, i8 %14, i64 %16, i1 false), !dbg !159
  %17 = ptrtoint i8* %11 to i64, !dbg !162
  store i64 %17, i64* %7, align 8, !dbg !163
  %18 = load i64, i64* %7, align 8, !dbg !164
  %19 = sitofp i64 %18 to double, !dbg !165
  ret double %19, !dbg !166
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #5

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define double @freed(double) #0 !dbg !167 {
  %2 = alloca double, align 8
  %3 = alloca i64, align 8
  store double %0, double* %2, align 8
  call void @llvm.dbg.declare(metadata double* %2, metadata !168, metadata !DIExpression()), !dbg !169
  call void @llvm.dbg.declare(metadata i64* %3, metadata !170, metadata !DIExpression()), !dbg !171
  %4 = load double, double* %2, align 8, !dbg !172
  %5 = fptosi double %4 to i64, !dbg !172
  store i64 %5, i64* %3, align 8, !dbg !171
  %6 = load i64, i64* %3, align 8, !dbg !173
  %7 = inttoptr i64 %6 to i8*, !dbg !174
  call void @free(i8* %7) #3, !dbg !175
  ret double 0.000000e+00, !dbg !176
}

; Function Attrs: nounwind
declare void @free(i8*) #4

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define double @cderef(double) #0 !dbg !177 {
  %2 = alloca double, align 8
  %3 = alloca i64, align 8
  store double %0, double* %2, align 8
  call void @llvm.dbg.declare(metadata double* %2, metadata !178, metadata !DIExpression()), !dbg !179
  call void @llvm.dbg.declare(metadata i64* %3, metadata !180, metadata !DIExpression()), !dbg !181
  %4 = load double, double* %2, align 8, !dbg !182
  %5 = fptosi double %4 to i64, !dbg !182
  store i64 %5, i64* %3, align 8, !dbg !181
  %6 = load i64, i64* %3, align 8, !dbg !183
  %7 = inttoptr i64 %6 to i8*, !dbg !184
  %8 = load i8, i8* %7, align 1, !dbg !185
  %9 = sitofp i8 %8 to double, !dbg !185
  ret double %9, !dbg !186
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define double @cderefset(double, double) #0 !dbg !187 {
  %3 = alloca double, align 8
  %4 = alloca double, align 8
  %5 = alloca i64, align 8
  store double %0, double* %3, align 8
  call void @llvm.dbg.declare(metadata double* %3, metadata !190, metadata !DIExpression()), !dbg !191
  store double %1, double* %4, align 8
  call void @llvm.dbg.declare(metadata double* %4, metadata !192, metadata !DIExpression()), !dbg !193
  call void @llvm.dbg.declare(metadata i64* %5, metadata !194, metadata !DIExpression()), !dbg !195
  %6 = load double, double* %3, align 8, !dbg !196
  %7 = fptosi double %6 to i64, !dbg !196
  store i64 %7, i64* %5, align 8, !dbg !195
  %8 = load i64, i64* %5, align 8, !dbg !197
  %9 = inttoptr i64 %8 to i8*, !dbg !198
  %10 = bitcast double* %4 to i8*, !dbg !199
  call void @llvm.memmove.p0i8.p0i8.i64(i8* align 1 %9, i8* align 1 %10, i64 1, i1 false), !dbg !200
  %11 = load double, double* %4, align 8, !dbg !201
  ret double %11, !dbg !202
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memmove.p0i8.p0i8.i64(i8* nocapture, i8* nocapture readonly, i64, i1 immarg) #5

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define double @dderef(double) #0 !dbg !203 {
  %2 = alloca double, align 8
  %3 = alloca i64, align 8
  store double %0, double* %2, align 8
  call void @llvm.dbg.declare(metadata double* %2, metadata !204, metadata !DIExpression()), !dbg !205
  call void @llvm.dbg.declare(metadata i64* %3, metadata !206, metadata !DIExpression()), !dbg !207
  %4 = load double, double* %2, align 8, !dbg !208
  %5 = fptosi double %4 to i64, !dbg !208
  store i64 %5, i64* %3, align 8, !dbg !207
  %6 = load i64, i64* %3, align 8, !dbg !209
  %7 = inttoptr i64 %6 to double*, !dbg !210
  %8 = load double, double* %7, align 8, !dbg !211
  ret double %8, !dbg !212
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define double @dderefset(double, double) #0 !dbg !213 {
  %3 = alloca double, align 8
  %4 = alloca double, align 8
  %5 = alloca i64, align 8
  store double %0, double* %3, align 8
  call void @llvm.dbg.declare(metadata double* %3, metadata !214, metadata !DIExpression()), !dbg !215
  store double %1, double* %4, align 8
  call void @llvm.dbg.declare(metadata double* %4, metadata !216, metadata !DIExpression()), !dbg !217
  call void @llvm.dbg.declare(metadata i64* %5, metadata !218, metadata !DIExpression()), !dbg !219
  %6 = load double, double* %3, align 8, !dbg !220
  %7 = fptosi double %6 to i64, !dbg !220
  store i64 %7, i64* %5, align 8, !dbg !219
  %8 = load i64, i64* %5, align 8, !dbg !221
  %9 = inttoptr i64 %8 to double*, !dbg !222
  %10 = bitcast double* %9 to i8*, !dbg !223
  %11 = bitcast double* %4 to i8*, !dbg !224
  call void @llvm.memmove.p0i8.p0i8.i64(i8* align 8 %10, i8* align 1 %11, i64 8, i1 false), !dbg !223
  %12 = load double, double* %4, align 8, !dbg !225
  ret double %12, !dbg !226
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define double @trap() #0 !dbg !227 {
  call void asm sideeffect "int3", "~{dirflag},~{fpsr},~{flags}"() #3, !dbg !230, !srcloc !231
  ret double 0.000000e+00, !dbg !232
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define double @repl_testmetadata(double) #0 !dbg !233 {
  %2 = alloca double, align 8
  %3 = alloca i32, align 4
  store double %0, double* %2, align 8
  call void @llvm.dbg.declare(metadata double* %2, metadata !234, metadata !DIExpression()), !dbg !235
  call void @llvm.dbg.declare(metadata i32* %3, metadata !236, metadata !DIExpression()), !dbg !237
  %4 = load double, double* %2, align 8, !dbg !238
  %5 = fptosi double %4 to i32, !dbg !238
  store i32 %5, i32* %3, align 4, !dbg !237
  %6 = load i32, i32* @metadata, align 4, !dbg !239
  %7 = load i32, i32* %3, align 4, !dbg !240
  %8 = shl i32 1, %7, !dbg !241
  %9 = and i32 %6, %8, !dbg !242
  %10 = icmp ne i32 %9, 0, !dbg !243
  %11 = xor i1 %10, true, !dbg !243
  %12 = xor i1 %11, true, !dbg !244
  %13 = zext i1 %12 to i32, !dbg !244
  %14 = sitofp i32 %13 to double, !dbg !245
  ret double %14, !dbg !246
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define i32 @main() #0 !dbg !247 {
  %1 = alloca i32, align 4
  %2 = alloca i64, align 8
  %3 = alloca i8*, align 8
  %4 = alloca i32, align 4
  %5 = alloca i64, align 8
  %6 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  call void @llvm.dbg.declare(metadata i64* %2, metadata !250, metadata !DIExpression()), !dbg !251
  store i64 1024000, i64* %2, align 8, !dbg !251
  call void @llvm.dbg.declare(metadata i8** %3, metadata !252, metadata !DIExpression()), !dbg !253
  %7 = load i64, i64* %2, align 8, !dbg !254
  %8 = call noalias i8* @malloc(i64 %7) #3, !dbg !255
  store i8* %8, i8** %3, align 8, !dbg !253
  %9 = load i8*, i8** %3, align 8, !dbg !256
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.2, i64 0, i64 0), %struct.sresult* @res, i8* %9), !dbg !257
  br label %11, !dbg !258

11:                                               ; preds = %0, %76
  call void @llvm.dbg.declare(metadata i32* %4, metadata !259, metadata !DIExpression()), !dbg !261
  store i32 -1, i32* %4, align 4, !dbg !261
  call void @llvm.dbg.declare(metadata i64* %5, metadata !262, metadata !DIExpression()), !dbg !263
  store i64 0, i64* %5, align 8, !dbg !263
  %12 = call i32 @isatty(i32 0) #3, !dbg !264
  %13 = icmp ne i32 %12, 0, !dbg !264
  br i1 %13, label %14, label %18, !dbg !266

14:                                               ; preds = %11
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.3, i64 0, i64 0)), !dbg !267
  %16 = load %struct._IO_FILE*, %struct._IO_FILE** @stdin, align 8, !dbg !269
  %17 = call i64 @getline(i8** %3, i64* %2, %struct._IO_FILE* %16), !dbg !270
  store i64 %17, i64* %5, align 8, !dbg !271
  br label %34, !dbg !272

18:                                               ; preds = %11
  %19 = load i8*, i8** %3, align 8, !dbg !273
  %20 = call i64 @read(i32 0, i8* %19, i64 1024000), !dbg !275
  store i64 %20, i64* %5, align 8, !dbg !276
  %21 = load i64, i64* %5, align 8, !dbg !277
  %22 = icmp ult i64 %21, 0, !dbg !279
  br i1 %22, label %23, label %29, !dbg !280

23:                                               ; preds = %18
  %24 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 8, !dbg !281
  %25 = call i32* @__errno_location() #7, !dbg !283
  %26 = load i32, i32* %25, align 4, !dbg !283
  %27 = call i8* @strerror(i32 %26) #3, !dbg !284
  %28 = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %24, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.4, i64 0, i64 0), i8* %27), !dbg !285
  br label %77, !dbg !286

29:                                               ; preds = %18
  %30 = load i64, i64* %5, align 8, !dbg !287
  %31 = icmp eq i64 %30, 0, !dbg !289
  br i1 %31, label %32, label %33, !dbg !290

32:                                               ; preds = %29
  br label %77, !dbg !291

33:                                               ; preds = %29
  br label %34

34:                                               ; preds = %33, %14
  %35 = load i8*, i8** %3, align 8, !dbg !292
  %36 = load i64, i64* %5, align 8, !dbg !293
  %37 = sub i64 %36, 1, !dbg !294
  %38 = getelementptr inbounds i8, i8* %35, i64 %37, !dbg !292
  store i8 0, i8* %38, align 1, !dbg !295
  %39 = load i8*, i8** %3, align 8, !dbg !296
  %40 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.5, i64 0, i64 0), i8* %39), !dbg !297
  %41 = load i8*, i8** %3, align 8, !dbg !298
  call void @__nlex_feed(i8* %41), !dbg !299
  br label %42, !dbg !300

42:                                               ; preds = %34, %74
  call void @__nlex_root(%struct.sresult* @res), !dbg !301
  %43 = load i8, i8* getelementptr inbounds (%struct.sresult, %struct.sresult* @res, i32 0, i32 3), align 8, !dbg !303
  %44 = sext i8 %43 to i32, !dbg !304
  %45 = icmp ne i32 %44, 0, !dbg !304
  %46 = zext i1 %45 to i64, !dbg !304
  %47 = select i1 %45, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.7, i64 0, i64 0), i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str.8, i64 0, i64 0), !dbg !304
  %48 = load i32, i32* getelementptr inbounds (%struct.sresult, %struct.sresult* @res, i32 0, i32 1), align 8, !dbg !305
  %49 = load i8*, i8** getelementptr inbounds (%struct.sresult, %struct.sresult* @res, i32 0, i32 0), align 8, !dbg !306
  %50 = load i8*, i8** getelementptr inbounds (%struct.sresult, %struct.sresult* @res, i32 0, i32 5), align 8, !dbg !307
  %51 = load i32, i32* getelementptr inbounds (%struct.sresult, %struct.sresult* @res, i32 0, i32 1), align 8, !dbg !308
  %52 = load i8*, i8** getelementptr inbounds (%struct.sresult, %struct.sresult* @res, i32 0, i32 2), align 8, !dbg !309
  %53 = load i8, i8* getelementptr inbounds (%struct.sresult, %struct.sresult* @res, i32 0, i32 4), align 1, !dbg !310
  %54 = zext i8 %53 to i32, !dbg !311
  %55 = and i32 %54, 1, !dbg !312
  %56 = icmp ne i32 %55, 0, !dbg !311
  %57 = zext i1 %56 to i64, !dbg !311
  %58 = select i1 %56, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.9, i64 0, i64 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.10, i64 0, i64 0), !dbg !311
  %59 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([46 x i8], [46 x i8]* @.str.6, i64 0, i64 0), i8* %47, i32 %48, i8* %49, i8* %50, i32 %51, i8* %52, i8* %58), !dbg !313
  %60 = load i8, i8* getelementptr inbounds (%struct.sresult, %struct.sresult* @res, i32 0, i32 4), align 1, !dbg !314
  %61 = zext i8 %60 to i32, !dbg !315
  store i32 %61, i32* @metadata, align 4, !dbg !316
  call void @llvm.dbg.declare(metadata i32* %6, metadata !317, metadata !DIExpression()), !dbg !318
  %62 = call i32 (...) @__nlex_distance(), !dbg !319
  store i32 %62, i32* %6, align 4, !dbg !318
  %63 = load i8, i8* getelementptr inbounds (%struct.sresult, %struct.sresult* @res, i32 0, i32 3), align 8, !dbg !320
  %64 = sext i8 %63 to i32, !dbg !322
  %65 = icmp ne i32 %64, 0, !dbg !322
  br i1 %65, label %73, label %66, !dbg !323

66:                                               ; preds = %42
  %67 = load i32, i32* getelementptr inbounds (%struct.sresult, %struct.sresult* @res, i32 0, i32 1), align 8, !dbg !324
  %68 = icmp eq i32 %67, 0, !dbg !325
  br i1 %68, label %73, label %69, !dbg !326

69:                                               ; preds = %66
  %70 = load i32, i32* %6, align 4, !dbg !327
  %71 = load i32, i32* %4, align 4, !dbg !328
  %72 = icmp eq i32 %70, %71, !dbg !329
  br i1 %72, label %73, label %74, !dbg !330

73:                                               ; preds = %69, %66, %42
  br label %76, !dbg !331

74:                                               ; preds = %69
  %75 = load i32, i32* %6, align 4, !dbg !332
  store i32 %75, i32* %4, align 4, !dbg !333
  br label %42, !dbg !300, !llvm.loop !334

76:                                               ; preds = %73
  br label %11, !dbg !258, !llvm.loop !336

77:                                               ; preds = %32, %23
  %78 = load i8*, i8** %3, align 8, !dbg !338
  call void @free(i8* %78) #3, !dbg !339
  %79 = load i32, i32* %1, align 4, !dbg !340
  ret i32 %79, !dbg !340
}

; Function Attrs: nounwind
declare i32 @isatty(i32) #4

declare i64 @getline(i8**, i64*, %struct._IO_FILE*) #2

declare i64 @read(i32, i8*, i64) #2

; Function Attrs: nounwind
declare i8* @strerror(i32) #4

; Function Attrs: nounwind readnone
declare i32* @__errno_location() #6

declare void @__nlex_feed(i8*) #2

declare void @__nlex_root(%struct.sresult*) #2

declare i32 @__nlex_distance(...) #2

attributes #0 = { noinline nounwind optnone sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }
attributes #4 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { argmemonly nounwind }
attributes #6 = { nounwind readnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #7 = { nounwind readnone }

!llvm.dbg.cu = !{!2}
!llvm.module.flags = !{!33, !34, !35, !36}
!llvm.ident = !{!37}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "res", scope: !2, file: !3, line: 16, type: !21, isLocal: false, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C99, file: !3, producer: "clang version 9.0.0 (tags/RELEASE_900/final)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !4, retainedTypes: !5, globals: !18, nameTableKind: None)
!3 = !DIFile(filename: "test.c", directory: "/home/Test/Documents/nlp-lex/src/playground")
!4 = !{}
!5 = !{!6, !7, !8, !9, !10, !13, !16, !17}
!6 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!7 = !DIBasicType(name: "double", size: 64, encoding: DW_ATE_float)
!8 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!9 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !6, size: 64)
!10 = !DIDerivedType(tag: DW_TAG_typedef, name: "size_t", file: !11, line: 46, baseType: !12)
!11 = !DIFile(filename: "/usr/lib/clang/9.0.0/include/stddef.h", directory: "")
!12 = !DIBasicType(name: "long unsigned int", size: 64, encoding: DW_ATE_unsigned)
!13 = !DIDerivedType(tag: DW_TAG_typedef, name: "__intptr_t", file: !14, line: 206, baseType: !15)
!14 = !DIFile(filename: "/usr/include/bits/types.h", directory: "")
!15 = !DIBasicType(name: "long int", size: 64, encoding: DW_ATE_signed)
!16 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64)
!17 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !7, size: 64)
!18 = !{!0, !19}
!19 = !DIGlobalVariableExpression(var: !20, expr: !DIExpression())
!20 = distinct !DIGlobalVariable(name: "metadata", scope: !2, file: !3, line: 242, type: !8, isLocal: true, isDefinition: true)
!21 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "sresult", file: !3, line: 8, size: 384, elements: !22)
!22 = !{!23, !26, !27, !28, !29, !31, !32}
!23 = !DIDerivedType(tag: DW_TAG_member, name: "start", scope: !21, file: !3, line: 9, baseType: !24, size: 64)
!24 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !25, size: 64)
!25 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !6)
!26 = !DIDerivedType(tag: DW_TAG_member, name: "length", scope: !21, file: !3, line: 10, baseType: !8, size: 32, offset: 64)
!27 = !DIDerivedType(tag: DW_TAG_member, name: "tag", scope: !21, file: !3, line: 11, baseType: !24, size: 64, offset: 128)
!28 = !DIDerivedType(tag: DW_TAG_member, name: "errc", scope: !21, file: !3, line: 12, baseType: !6, size: 8, offset: 192)
!29 = !DIDerivedType(tag: DW_TAG_member, name: "metadata", scope: !21, file: !3, line: 13, baseType: !30, size: 8, offset: 200)
!30 = !DIBasicType(name: "unsigned char", size: 8, encoding: DW_ATE_unsigned_char)
!31 = !DIDerivedType(tag: DW_TAG_member, name: "pos", scope: !21, file: !3, line: 14, baseType: !24, size: 64, offset: 256)
!32 = !DIDerivedType(tag: DW_TAG_member, name: "allocd", scope: !21, file: !3, line: 15, baseType: !8, size: 32, offset: 320)
!33 = !{i32 2, !"Dwarf Version", i32 4}
!34 = !{i32 2, !"Debug Info Version", i32 3}
!35 = !{i32 1, !"wchar_size", i32 4}
!36 = !{i32 7, !"PIC Level", i32 1}
!37 = !{!"clang version 9.0.0 (tags/RELEASE_900/final)"}
!38 = distinct !DISubprogram(name: "putchard", scope: !3, file: !3, line: 166, type: !39, scopeLine: 166, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!39 = !DISubroutineType(types: !40)
!40 = !{!7, !7}
!41 = !DILocalVariable(name: "d", arg: 1, scope: !38, file: !3, line: 166, type: !7)
!42 = !DILocation(line: 166, column: 24, scope: !38)
!43 = !DILocalVariable(name: "c", scope: !38, file: !3, line: 167, type: !6)
!44 = !DILocation(line: 167, column: 8, scope: !38)
!45 = !DILocation(line: 167, column: 19, scope: !38)
!46 = !DILocation(line: 167, column: 12, scope: !38)
!47 = !DILocalVariable(name: "x", scope: !38, file: !3, line: 168, type: !8)
!48 = !DILocation(line: 168, column: 7, scope: !38)
!49 = !DILocation(line: 168, column: 19, scope: !38)
!50 = !DILocation(line: 168, column: 33, scope: !38)
!51 = !DILocation(line: 168, column: 11, scope: !38)
!52 = !DILocation(line: 169, column: 19, scope: !38)
!53 = !DILocation(line: 169, column: 10, scope: !38)
!54 = !DILocation(line: 169, column: 3, scope: !38)
!55 = distinct !DISubprogram(name: "eputchard", scope: !3, file: !3, line: 172, type: !39, scopeLine: 172, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!56 = !DILocalVariable(name: "d", arg: 1, scope: !55, file: !3, line: 172, type: !7)
!57 = !DILocation(line: 172, column: 25, scope: !55)
!58 = !DILocalVariable(name: "c", scope: !55, file: !3, line: 173, type: !6)
!59 = !DILocation(line: 173, column: 8, scope: !55)
!60 = !DILocation(line: 173, column: 19, scope: !55)
!61 = !DILocation(line: 173, column: 12, scope: !55)
!62 = !DILocalVariable(name: "x", scope: !55, file: !3, line: 174, type: !8)
!63 = !DILocation(line: 174, column: 7, scope: !55)
!64 = !DILocation(line: 174, column: 19, scope: !55)
!65 = !DILocation(line: 174, column: 33, scope: !55)
!66 = !DILocation(line: 174, column: 11, scope: !55)
!67 = !DILocation(line: 175, column: 19, scope: !55)
!68 = !DILocation(line: 175, column: 10, scope: !55)
!69 = !DILocation(line: 175, column: 3, scope: !55)
!70 = distinct !DISubprogram(name: "printd", scope: !3, file: !3, line: 178, type: !39, scopeLine: 178, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!71 = !DILocalVariable(name: "d", arg: 1, scope: !70, file: !3, line: 178, type: !7)
!72 = !DILocation(line: 178, column: 22, scope: !70)
!73 = !DILocation(line: 179, column: 33, scope: !70)
!74 = !DILocation(line: 179, column: 19, scope: !70)
!75 = !DILocation(line: 179, column: 10, scope: !70)
!76 = !DILocation(line: 179, column: 3, scope: !70)
!77 = distinct !DISubprogram(name: "eprintd", scope: !3, file: !3, line: 182, type: !39, scopeLine: 182, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!78 = !DILocalVariable(name: "d", arg: 1, scope: !77, file: !3, line: 182, type: !7)
!79 = !DILocation(line: 182, column: 23, scope: !77)
!80 = !DILocation(line: 183, column: 27, scope: !77)
!81 = !DILocation(line: 183, column: 42, scope: !77)
!82 = !DILocation(line: 183, column: 19, scope: !77)
!83 = !DILocation(line: 183, column: 10, scope: !77)
!84 = !DILocation(line: 183, column: 3, scope: !77)
!85 = distinct !DISubprogram(name: "dprintdf", scope: !3, file: !3, line: 186, type: !86, scopeLine: 186, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!86 = !DISubroutineType(types: !87)
!87 = !{!7, !7, !7, null}
!88 = !DILocalVariable(name: "fd", arg: 1, scope: !85, file: !3, line: 186, type: !7)
!89 = !DILocation(line: 186, column: 24, scope: !85)
!90 = !DILocalVariable(name: "ptr", arg: 2, scope: !85, file: !3, line: 186, type: !7)
!91 = !DILocation(line: 186, column: 35, scope: !85)
!92 = !DILocalVariable(name: "ap", scope: !85, file: !3, line: 187, type: !93)
!93 = !DIDerivedType(tag: DW_TAG_typedef, name: "va_list", file: !94, line: 52, baseType: !95)
!94 = !DIFile(filename: "/usr/include/stdio.h", directory: "")
!95 = !DIDerivedType(tag: DW_TAG_typedef, name: "__gnuc_va_list", file: !96, line: 32, baseType: !97)
!96 = !DIFile(filename: "/usr/lib/clang/9.0.0/include/stdarg.h", directory: "")
!97 = !DIDerivedType(tag: DW_TAG_typedef, name: "__builtin_va_list", file: !3, line: 187, baseType: !98)
!98 = !DICompositeType(tag: DW_TAG_array_type, baseType: !99, size: 192, elements: !106)
!99 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__va_list_tag", file: !3, line: 187, size: 192, elements: !100)
!100 = !{!101, !103, !104, !105}
!101 = !DIDerivedType(tag: DW_TAG_member, name: "gp_offset", scope: !99, file: !3, line: 187, baseType: !102, size: 32)
!102 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!103 = !DIDerivedType(tag: DW_TAG_member, name: "fp_offset", scope: !99, file: !3, line: 187, baseType: !102, size: 32, offset: 32)
!104 = !DIDerivedType(tag: DW_TAG_member, name: "overflow_arg_area", scope: !99, file: !3, line: 187, baseType: !16, size: 64, offset: 64)
!105 = !DIDerivedType(tag: DW_TAG_member, name: "reg_save_area", scope: !99, file: !3, line: 187, baseType: !16, size: 64, offset: 128)
!106 = !{!107}
!107 = !DISubrange(count: 1)
!108 = !DILocation(line: 187, column: 11, scope: !85)
!109 = !DILocation(line: 188, column: 3, scope: !85)
!110 = !DILocalVariable(name: "ip", scope: !85, file: !3, line: 190, type: !13)
!111 = !DILocation(line: 190, column: 14, scope: !85)
!112 = !DILocation(line: 190, column: 19, scope: !85)
!113 = !DILocalVariable(name: "res", scope: !85, file: !3, line: 191, type: !7)
!114 = !DILocation(line: 191, column: 10, scope: !85)
!115 = !DILocation(line: 191, column: 40, scope: !85)
!116 = !DILocation(line: 191, column: 34, scope: !85)
!117 = !DILocation(line: 191, column: 52, scope: !85)
!118 = !DILocation(line: 191, column: 44, scope: !85)
!119 = !DILocation(line: 191, column: 56, scope: !85)
!120 = !DILocation(line: 191, column: 25, scope: !85)
!121 = !DILocation(line: 191, column: 16, scope: !85)
!122 = !DILocation(line: 192, column: 3, scope: !85)
!123 = !DILocation(line: 193, column: 10, scope: !85)
!124 = !DILocation(line: 193, column: 3, scope: !85)
!125 = distinct !DISubprogram(name: "mallocd", scope: !3, file: !3, line: 196, type: !39, scopeLine: 196, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!126 = !DILocalVariable(name: "s", arg: 1, scope: !125, file: !3, line: 196, type: !7)
!127 = !DILocation(line: 196, column: 23, scope: !125)
!128 = !DILocalVariable(name: "sze", scope: !125, file: !3, line: 197, type: !10)
!129 = !DILocation(line: 197, column: 10, scope: !125)
!130 = !DILocation(line: 197, column: 25, scope: !125)
!131 = !DILocation(line: 197, column: 16, scope: !125)
!132 = !DILocalVariable(name: "mem", scope: !125, file: !3, line: 198, type: !16)
!133 = !DILocation(line: 198, column: 9, scope: !125)
!134 = !DILocation(line: 198, column: 22, scope: !125)
!135 = !DILocation(line: 198, column: 15, scope: !125)
!136 = !DILocalVariable(name: "ip", scope: !125, file: !3, line: 199, type: !13)
!137 = !DILocation(line: 199, column: 14, scope: !125)
!138 = !DILocation(line: 199, column: 32, scope: !125)
!139 = !DILocation(line: 199, column: 19, scope: !125)
!140 = !DILocation(line: 200, column: 19, scope: !125)
!141 = !DILocation(line: 200, column: 10, scope: !125)
!142 = !DILocation(line: 200, column: 3, scope: !125)
!143 = distinct !DISubprogram(name: "memsetd", scope: !3, file: !3, line: 203, type: !144, scopeLine: 203, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!144 = !DISubroutineType(types: !145)
!145 = !{!7, !7, !7, !7}
!146 = !DILocalVariable(name: "p", arg: 1, scope: !143, file: !3, line: 203, type: !7)
!147 = !DILocation(line: 203, column: 23, scope: !143)
!148 = !DILocalVariable(name: "s", arg: 2, scope: !143, file: !3, line: 203, type: !7)
!149 = !DILocation(line: 203, column: 33, scope: !143)
!150 = !DILocalVariable(name: "n", arg: 3, scope: !143, file: !3, line: 203, type: !7)
!151 = !DILocation(line: 203, column: 43, scope: !143)
!152 = !DILocalVariable(name: "ip", scope: !143, file: !3, line: 204, type: !13)
!153 = !DILocation(line: 204, column: 14, scope: !143)
!154 = !DILocation(line: 204, column: 19, scope: !143)
!155 = !DILocation(line: 205, column: 36, scope: !143)
!156 = !DILocation(line: 205, column: 28, scope: !143)
!157 = !DILocation(line: 205, column: 46, scope: !143)
!158 = !DILocation(line: 205, column: 40, scope: !143)
!159 = !DILocation(line: 205, column: 21, scope: !143)
!160 = !DILocation(line: 205, column: 58, scope: !143)
!161 = !DILocation(line: 205, column: 49, scope: !143)
!162 = !DILocation(line: 205, column: 8, scope: !143)
!163 = !DILocation(line: 205, column: 6, scope: !143)
!164 = !DILocation(line: 206, column: 19, scope: !143)
!165 = !DILocation(line: 206, column: 10, scope: !143)
!166 = !DILocation(line: 206, column: 3, scope: !143)
!167 = distinct !DISubprogram(name: "freed", scope: !3, file: !3, line: 209, type: !39, scopeLine: 209, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!168 = !DILocalVariable(name: "ptr", arg: 1, scope: !167, file: !3, line: 209, type: !7)
!169 = !DILocation(line: 209, column: 21, scope: !167)
!170 = !DILocalVariable(name: "p", scope: !167, file: !3, line: 210, type: !13)
!171 = !DILocation(line: 210, column: 14, scope: !167)
!172 = !DILocation(line: 210, column: 18, scope: !167)
!173 = !DILocation(line: 211, column: 16, scope: !167)
!174 = !DILocation(line: 211, column: 8, scope: !167)
!175 = !DILocation(line: 211, column: 3, scope: !167)
!176 = !DILocation(line: 212, column: 3, scope: !167)
!177 = distinct !DISubprogram(name: "cderef", scope: !3, file: !3, line: 215, type: !39, scopeLine: 215, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!178 = !DILocalVariable(name: "ptr", arg: 1, scope: !177, file: !3, line: 215, type: !7)
!179 = !DILocation(line: 215, column: 22, scope: !177)
!180 = !DILocalVariable(name: "p", scope: !177, file: !3, line: 216, type: !13)
!181 = !DILocation(line: 216, column: 14, scope: !177)
!182 = !DILocation(line: 216, column: 18, scope: !177)
!183 = !DILocation(line: 217, column: 21, scope: !177)
!184 = !DILocation(line: 217, column: 13, scope: !177)
!185 = !DILocation(line: 217, column: 10, scope: !177)
!186 = !DILocation(line: 217, column: 3, scope: !177)
!187 = distinct !DISubprogram(name: "cderefset", scope: !3, file: !3, line: 220, type: !188, scopeLine: 220, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!188 = !DISubroutineType(types: !189)
!189 = !{!7, !7, !7}
!190 = !DILocalVariable(name: "ptr", arg: 1, scope: !187, file: !3, line: 220, type: !7)
!191 = !DILocation(line: 220, column: 25, scope: !187)
!192 = !DILocalVariable(name: "value", arg: 2, scope: !187, file: !3, line: 220, type: !7)
!193 = !DILocation(line: 220, column: 37, scope: !187)
!194 = !DILocalVariable(name: "p", scope: !187, file: !3, line: 221, type: !13)
!195 = !DILocation(line: 221, column: 14, scope: !187)
!196 = !DILocation(line: 221, column: 18, scope: !187)
!197 = !DILocation(line: 222, column: 19, scope: !187)
!198 = !DILocation(line: 222, column: 11, scope: !187)
!199 = !DILocation(line: 222, column: 22, scope: !187)
!200 = !DILocation(line: 222, column: 3, scope: !187)
!201 = !DILocation(line: 223, column: 10, scope: !187)
!202 = !DILocation(line: 223, column: 3, scope: !187)
!203 = distinct !DISubprogram(name: "dderef", scope: !3, file: !3, line: 226, type: !39, scopeLine: 226, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!204 = !DILocalVariable(name: "ptr", arg: 1, scope: !203, file: !3, line: 226, type: !7)
!205 = !DILocation(line: 226, column: 22, scope: !203)
!206 = !DILocalVariable(name: "p", scope: !203, file: !3, line: 227, type: !13)
!207 = !DILocation(line: 227, column: 14, scope: !203)
!208 = !DILocation(line: 227, column: 18, scope: !203)
!209 = !DILocation(line: 228, column: 23, scope: !203)
!210 = !DILocation(line: 228, column: 13, scope: !203)
!211 = !DILocation(line: 228, column: 10, scope: !203)
!212 = !DILocation(line: 228, column: 3, scope: !203)
!213 = distinct !DISubprogram(name: "dderefset", scope: !3, file: !3, line: 231, type: !188, scopeLine: 231, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!214 = !DILocalVariable(name: "ptr", arg: 1, scope: !213, file: !3, line: 231, type: !7)
!215 = !DILocation(line: 231, column: 25, scope: !213)
!216 = !DILocalVariable(name: "value", arg: 2, scope: !213, file: !3, line: 231, type: !7)
!217 = !DILocation(line: 231, column: 37, scope: !213)
!218 = !DILocalVariable(name: "p", scope: !213, file: !3, line: 232, type: !13)
!219 = !DILocation(line: 232, column: 14, scope: !213)
!220 = !DILocation(line: 232, column: 18, scope: !213)
!221 = !DILocation(line: 233, column: 21, scope: !213)
!222 = !DILocation(line: 233, column: 11, scope: !213)
!223 = !DILocation(line: 233, column: 3, scope: !213)
!224 = !DILocation(line: 233, column: 24, scope: !213)
!225 = !DILocation(line: 234, column: 10, scope: !213)
!226 = !DILocation(line: 234, column: 3, scope: !213)
!227 = distinct !DISubprogram(name: "trap", scope: !3, file: !3, line: 237, type: !228, scopeLine: 237, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!228 = !DISubroutineType(types: !229)
!229 = !{!7}
!230 = !DILocation(line: 238, column: 3, scope: !227)
!231 = !{i32 5571}
!232 = !DILocation(line: 239, column: 3, scope: !227)
!233 = distinct !DISubprogram(name: "repl_testmetadata", scope: !3, file: !3, line: 244, type: !39, scopeLine: 244, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!234 = !DILocalVariable(name: "bit", arg: 1, scope: !233, file: !3, line: 244, type: !7)
!235 = !DILocation(line: 244, column: 33, scope: !233)
!236 = !DILocalVariable(name: "ibit", scope: !233, file: !3, line: 245, type: !8)
!237 = !DILocation(line: 245, column: 7, scope: !233)
!238 = !DILocation(line: 245, column: 14, scope: !233)
!239 = !DILocation(line: 246, column: 22, scope: !233)
!240 = !DILocation(line: 246, column: 35, scope: !233)
!241 = !DILocation(line: 246, column: 33, scope: !233)
!242 = !DILocation(line: 246, column: 30, scope: !233)
!243 = !DILocation(line: 246, column: 20, scope: !233)
!244 = !DILocation(line: 246, column: 19, scope: !233)
!245 = !DILocation(line: 246, column: 10, scope: !233)
!246 = !DILocation(line: 246, column: 3, scope: !233)
!247 = distinct !DISubprogram(name: "main", scope: !3, file: !3, line: 250, type: !248, scopeLine: 250, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!248 = !DISubroutineType(types: !249)
!249 = !{!8}
!250 = !DILocalVariable(name: "size", scope: !247, file: !3, line: 252, type: !10)
!251 = !DILocation(line: 252, column: 10, scope: !247)
!252 = !DILocalVariable(name: "s", scope: !247, file: !3, line: 253, type: !9)
!253 = !DILocation(line: 253, column: 9, scope: !247)
!254 = !DILocation(line: 253, column: 20, scope: !247)
!255 = !DILocation(line: 253, column: 13, scope: !247)
!256 = !DILocation(line: 254, column: 40, scope: !247)
!257 = !DILocation(line: 254, column: 3, scope: !247)
!258 = !DILocation(line: 255, column: 3, scope: !247)
!259 = !DILocalVariable(name: "last", scope: !260, file: !3, line: 256, type: !8)
!260 = distinct !DILexicalBlock(scope: !247, file: !3, line: 255, column: 13)
!261 = !DILocation(line: 256, column: 9, scope: !260)
!262 = !DILocalVariable(name: "els", scope: !260, file: !3, line: 257, type: !10)
!263 = !DILocation(line: 257, column: 12, scope: !260)
!264 = !DILocation(line: 258, column: 9, scope: !265)
!265 = distinct !DILexicalBlock(scope: !260, file: !3, line: 258, column: 9)
!266 = !DILocation(line: 258, column: 9, scope: !260)
!267 = !DILocation(line: 259, column: 9, scope: !268)
!268 = distinct !DILexicalBlock(scope: !265, file: !3, line: 258, column: 31)
!269 = !DILocation(line: 260, column: 34, scope: !268)
!270 = !DILocation(line: 260, column: 15, scope: !268)
!271 = !DILocation(line: 260, column: 13, scope: !268)
!272 = !DILocation(line: 261, column: 5, scope: !268)
!273 = !DILocation(line: 262, column: 34, scope: !274)
!274 = distinct !DILexicalBlock(scope: !265, file: !3, line: 261, column: 12)
!275 = !DILocation(line: 262, column: 15, scope: !274)
!276 = !DILocation(line: 262, column: 13, scope: !274)
!277 = !DILocation(line: 263, column: 13, scope: !278)
!278 = distinct !DILexicalBlock(scope: !274, file: !3, line: 263, column: 13)
!279 = !DILocation(line: 263, column: 17, scope: !278)
!280 = !DILocation(line: 263, column: 13, scope: !274)
!281 = !DILocation(line: 264, column: 21, scope: !282)
!282 = distinct !DILexicalBlock(scope: !278, file: !3, line: 263, column: 22)
!283 = !DILocation(line: 264, column: 64, scope: !282)
!284 = !DILocation(line: 264, column: 55, scope: !282)
!285 = !DILocation(line: 264, column: 13, scope: !282)
!286 = !DILocation(line: 265, column: 13, scope: !282)
!287 = !DILocation(line: 267, column: 13, scope: !288)
!288 = distinct !DILexicalBlock(scope: !274, file: !3, line: 267, column: 13)
!289 = !DILocation(line: 267, column: 17, scope: !288)
!290 = !DILocation(line: 267, column: 13, scope: !274)
!291 = !DILocation(line: 267, column: 23, scope: !288)
!292 = !DILocation(line: 269, column: 5, scope: !260)
!293 = !DILocation(line: 269, column: 7, scope: !260)
!294 = !DILocation(line: 269, column: 11, scope: !260)
!295 = !DILocation(line: 269, column: 16, scope: !260)
!296 = !DILocation(line: 270, column: 35, scope: !260)
!297 = !DILocation(line: 270, column: 5, scope: !260)
!298 = !DILocation(line: 272, column: 17, scope: !260)
!299 = !DILocation(line: 272, column: 5, scope: !260)
!300 = !DILocation(line: 273, column: 5, scope: !260)
!301 = !DILocation(line: 275, column: 7, scope: !302)
!302 = distinct !DILexicalBlock(scope: !260, file: !3, line: 273, column: 48)
!303 = !DILocation(line: 278, column: 19, scope: !302)
!304 = !DILocation(line: 278, column: 15, scope: !302)
!305 = !DILocation(line: 278, column: 43, scope: !302)
!306 = !DILocation(line: 278, column: 55, scope: !302)
!307 = !DILocation(line: 278, column: 66, scope: !302)
!308 = !DILocation(line: 279, column: 18, scope: !302)
!309 = !DILocation(line: 279, column: 30, scope: !302)
!310 = !DILocation(line: 279, column: 40, scope: !302)
!311 = !DILocation(line: 279, column: 36, scope: !302)
!312 = !DILocation(line: 279, column: 49, scope: !302)
!313 = !DILocation(line: 277, column: 7, scope: !302)
!314 = !DILocation(line: 280, column: 22, scope: !302)
!315 = !DILocation(line: 280, column: 18, scope: !302)
!316 = !DILocation(line: 280, column: 16, scope: !302)
!317 = !DILocalVariable(name: "dist", scope: !302, file: !3, line: 281, type: !8)
!318 = !DILocation(line: 281, column: 11, scope: !302)
!319 = !DILocation(line: 281, column: 18, scope: !302)
!320 = !DILocation(line: 282, column: 15, scope: !321)
!321 = distinct !DILexicalBlock(scope: !302, file: !3, line: 282, column: 11)
!322 = !DILocation(line: 282, column: 11, scope: !321)
!323 = !DILocation(line: 282, column: 20, scope: !321)
!324 = !DILocation(line: 282, column: 27, scope: !321)
!325 = !DILocation(line: 282, column: 34, scope: !321)
!326 = !DILocation(line: 282, column: 39, scope: !321)
!327 = !DILocation(line: 282, column: 42, scope: !321)
!328 = !DILocation(line: 282, column: 50, scope: !321)
!329 = !DILocation(line: 282, column: 47, scope: !321)
!330 = !DILocation(line: 282, column: 11, scope: !302)
!331 = !DILocation(line: 283, column: 9, scope: !321)
!332 = !DILocation(line: 284, column: 14, scope: !302)
!333 = !DILocation(line: 284, column: 12, scope: !302)
!334 = distinct !{!334, !300, !335}
!335 = !DILocation(line: 285, column: 5, scope: !260)
!336 = distinct !{!336, !258, !337}
!337 = !DILocation(line: 286, column: 3, scope: !247)
!338 = !DILocation(line: 287, column: 8, scope: !247)
!339 = !DILocation(line: 287, column: 3, scope: !247)
!340 = !DILocation(line: 289, column: 1, scope: !247)
