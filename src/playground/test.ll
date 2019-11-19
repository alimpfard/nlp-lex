; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.WorkingBuffer = type { %struct.sresult*, i32, i32, i32, i32 }
%struct.sresult = type { i8*, i32, i8*, i8, i8, i8*, i32 }
%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i64, i16, i8, [1 x i8], i8*, i64, %struct._IO_codecvt*, %struct._IO_wide_data*, %struct._IO_FILE*, i8*, i64, i32, [20 x i8] }
%struct._IO_marker = type opaque
%struct._IO_codecvt = type opaque
%struct._IO_wide_data = type opaque

@front = internal global %struct.WorkingBuffer { %struct.sresult* getelementptr inbounds ([1024 x %struct.sresult], [1024 x %struct.sresult]* @bufferB, i32 0, i32 0), i32 1024, i32 0, i32 0, i32 0 }, align 8, !dbg !0
@dumpout = internal global i32 0, align 4, !dbg !27
@done = internal global i32 0, align 4, !dbg !29
@back = internal global %struct.WorkingBuffer { %struct.sresult* getelementptr inbounds ([1024 x %struct.sresult], [1024 x %struct.sresult]* @bufferA, i32 0, i32 0), i32 1024, i32 0, i32 0, i32 0 }, align 8, !dbg !31
@.str = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@fst = internal global i32 1, align 4, !dbg !41
@.str.1 = private unnamed_addr constant [20 x i8] c"res at %p, s at %p\0A\00", align 1
@.str.2 = private unnamed_addr constant [3 x i8] c"> \00", align 1
@stdin = external global %struct._IO_FILE*, align 8
@.str.3 = private unnamed_addr constant [19 x i8] c"processing - '%s'\0A\00", align 1
@.str.4 = private unnamed_addr constant [18 x i8] c"SentenceDelimiter\00", align 1
@.str.5 = private unnamed_addr constant [46 x i8] c"%smatch {'%.*s' - %s - %d %s} is%sa stopword\0A\00", align 1
@.str.6 = private unnamed_addr constant [4 x i8] c"no \00", align 1
@.str.7 = private unnamed_addr constant [2 x i8] c" \00", align 1
@.str.8 = private unnamed_addr constant [6 x i8] c" not \00", align 1
@bufferA = common dso_local global [1024 x %struct.sresult] zeroinitializer, align 16, !dbg !6
@bufferB = common dso_local global [1024 x %struct.sresult] zeroinitializer, align 16, !dbg !25

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @__nlex_postag_m(%struct.sresult*, i8*) #0 !dbg !49 {
  %3 = alloca %struct.sresult*, align 8
  %4 = alloca i8*, align 8
  %5 = alloca i32, align 4
  %6 = alloca %struct.sresult, align 8
  %7 = alloca %struct.sresult*, align 8
  store %struct.sresult* %0, %struct.sresult** %3, align 8
  call void @llvm.dbg.declare(metadata %struct.sresult** %3, metadata !52, metadata !DIExpression()), !dbg !53
  store i8* %1, i8** %4, align 8
  call void @llvm.dbg.declare(metadata i8** %4, metadata !54, metadata !DIExpression()), !dbg !55
  call void @llvm.dbg.declare(metadata i32* %5, metadata !56, metadata !DIExpression()), !dbg !57
  store i32 0, i32* %5, align 4, !dbg !57
  %8 = load i32, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @front, i32 0, i32 4), align 4, !dbg !58
  %9 = icmp ne i32 %8, 0, !dbg !60
  br i1 %9, label %10, label %28, !dbg !61

10:                                               ; preds = %2
  %11 = load %struct.sresult*, %struct.sresult** %3, align 8, !dbg !62
  %12 = load %struct.sresult*, %struct.sresult** getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @front, i32 0, i32 0), align 8, !dbg !64
  %13 = load i32, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @front, i32 0, i32 2), align 4, !dbg !65
  %14 = add nsw i32 %13, 1, !dbg !65
  store i32 %14, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @front, i32 0, i32 2), align 4, !dbg !65
  %15 = sext i32 %13 to i64, !dbg !66
  %16 = getelementptr inbounds %struct.sresult, %struct.sresult* %12, i64 %15, !dbg !66
  %17 = bitcast %struct.sresult* %11 to i8*, !dbg !66
  %18 = bitcast %struct.sresult* %16 to i8*, !dbg !66
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %17, i8* align 8 %18, i64 48, i1 false), !dbg !66
  %19 = load i32, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @front, i32 0, i32 2), align 4, !dbg !67
  %20 = load i32, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @front, i32 0, i32 3), align 8, !dbg !69
  %21 = icmp sge i32 %19, %20, !dbg !70
  br i1 %21, label %22, label %23, !dbg !71

22:                                               ; preds = %10
  store i32 0, i32* @dumpout, align 4, !dbg !72
  call void @resetfront(), !dbg !74
  store i32 1, i32* %5, align 4, !dbg !75
  br label %23, !dbg !76

23:                                               ; preds = %22, %10
  %24 = load i32, i32* @dumpout, align 4, !dbg !77
  %25 = icmp ne i32 %24, 0, !dbg !77
  br i1 %25, label %26, label %27, !dbg !79

26:                                               ; preds = %23
  br label %100, !dbg !80

27:                                               ; preds = %23
  br label %29, !dbg !81

28:                                               ; preds = %2
  store i32 1, i32* %5, align 4, !dbg !82
  br label %29

29:                                               ; preds = %28, %27
  %30 = load i32, i32* @done, align 4, !dbg !83
  %31 = icmp ne i32 %30, 0, !dbg !83
  br i1 %31, label %32, label %33, !dbg !85

32:                                               ; preds = %29
  br label %100, !dbg !86

33:                                               ; preds = %29
  %34 = load i32, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @back, i32 0, i32 4), align 4, !dbg !87
  %35 = icmp ne i32 %34, 0, !dbg !89
  br i1 %35, label %36, label %51, !dbg !90

36:                                               ; preds = %33
  call void @swap_buffers(), !dbg !91
  call void @resetback(), !dbg !93
  call void (...) @__nlex_generated_postag(), !dbg !94
  %37 = load i32, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @front, i32 0, i32 2), align 4, !dbg !95
  %38 = load i32, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @front, i32 0, i32 3), align 8, !dbg !97
  %39 = icmp slt i32 %37, %38, !dbg !98
  br i1 %39, label %40, label %49, !dbg !99

40:                                               ; preds = %36
  %41 = load %struct.sresult*, %struct.sresult** %3, align 8, !dbg !100
  %42 = load %struct.sresult*, %struct.sresult** getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @front, i32 0, i32 0), align 8, !dbg !101
  %43 = load i32, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @front, i32 0, i32 2), align 4, !dbg !102
  %44 = add nsw i32 %43, 1, !dbg !102
  store i32 %44, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @front, i32 0, i32 2), align 4, !dbg !102
  %45 = sext i32 %43 to i64, !dbg !103
  %46 = getelementptr inbounds %struct.sresult, %struct.sresult* %42, i64 %45, !dbg !103
  %47 = bitcast %struct.sresult* %41 to i8*, !dbg !103
  %48 = bitcast %struct.sresult* %46 to i8*, !dbg !103
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %47, i8* align 8 %48, i64 48, i1 false), !dbg !103
  br label %50, !dbg !104

49:                                               ; preds = %36
  store i32 0, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @front, i32 0, i32 4), align 4, !dbg !105
  br label %50

50:                                               ; preds = %49, %40
  br label %100, !dbg !107

51:                                               ; preds = %33
  call void @llvm.dbg.declare(metadata %struct.sresult* %6, metadata !108, metadata !DIExpression()), !dbg !110
  %52 = bitcast %struct.sresult* %6 to i8*, !dbg !110
  call void @llvm.memset.p0i8.i64(i8* align 8 %52, i8 0, i64 48, i1 false), !dbg !110
  call void @__nlex_root(%struct.sresult* %6), !dbg !111
  %53 = getelementptr inbounds %struct.sresult, %struct.sresult* %6, i32 0, i32 3, !dbg !112
  %54 = load i8, i8* %53, align 8, !dbg !112
  %55 = sext i8 %54 to i32, !dbg !114
  %56 = icmp ne i32 %55, 0, !dbg !114
  br i1 %56, label %61, label %57, !dbg !115

57:                                               ; preds = %51
  %58 = getelementptr inbounds %struct.sresult, %struct.sresult* %6, i32 0, i32 1, !dbg !116
  %59 = load i32, i32* %58, align 8, !dbg !116
  %60 = icmp eq i32 %59, 0, !dbg !117
  br i1 %60, label %61, label %64, !dbg !118

61:                                               ; preds = %57, %51
  %62 = getelementptr inbounds %struct.sresult, %struct.sresult* %6, i32 0, i32 0, !dbg !119
  store i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i64 0, i64 0), i8** %62, align 8, !dbg !121
  %63 = getelementptr inbounds %struct.sresult, %struct.sresult* %6, i32 0, i32 1, !dbg !122
  store i32 0, i32* %63, align 8, !dbg !123
  store i32 1, i32* @done, align 4, !dbg !124
  br label %69, !dbg !125

64:                                               ; preds = %57
  %65 = getelementptr inbounds %struct.sresult, %struct.sresult* %6, i32 0, i32 0, !dbg !126
  %66 = load i8*, i8** %65, align 8, !dbg !126
  %67 = call i8* @strdup(i8* %66), !dbg !127
  %68 = getelementptr inbounds %struct.sresult, %struct.sresult* %6, i32 0, i32 0, !dbg !128
  store i8* %67, i8** %68, align 8, !dbg !129
  br label %69

69:                                               ; preds = %64, %61
  call void @llvm.dbg.declare(metadata %struct.sresult** %7, metadata !130, metadata !DIExpression()), !dbg !131
  %70 = load %struct.sresult*, %struct.sresult** getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @back, i32 0, i32 0), align 8, !dbg !132
  %71 = load i32, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @back, i32 0, i32 3), align 8, !dbg !133
  %72 = add nsw i32 %71, 1, !dbg !133
  store i32 %72, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @back, i32 0, i32 3), align 8, !dbg !133
  %73 = sext i32 %71 to i64, !dbg !134
  %74 = getelementptr inbounds %struct.sresult, %struct.sresult* %70, i64 %73, !dbg !134
  store %struct.sresult* %74, %struct.sresult** %7, align 8, !dbg !131
  %75 = load %struct.sresult*, %struct.sresult** %7, align 8, !dbg !135
  %76 = getelementptr inbounds %struct.sresult, %struct.sresult* %75, i32 0, i32 6, !dbg !137
  %77 = load i32, i32* %76, align 8, !dbg !137
  %78 = icmp ne i32 %77, 0, !dbg !135
  br i1 %78, label %79, label %84, !dbg !138

79:                                               ; preds = %69
  %80 = load %struct.sresult*, %struct.sresult** %7, align 8, !dbg !139
  %81 = getelementptr inbounds %struct.sresult, %struct.sresult* %80, i32 0, i32 0, !dbg !141
  %82 = load i8*, i8** %81, align 8, !dbg !141
  %83 = call i32 (i8*, ...) bitcast (i32 (...)* @free to i32 (i8*, ...)*)(i8* %82), !dbg !142
  br label %84, !dbg !143

84:                                               ; preds = %79, %69
  %85 = load %struct.sresult*, %struct.sresult** %7, align 8, !dbg !144
  %86 = bitcast %struct.sresult* %85 to i8*, !dbg !145
  %87 = bitcast %struct.sresult* %6 to i8*, !dbg !145
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %86, i8* align 8 %87, i64 48, i1 false), !dbg !145
  %88 = getelementptr inbounds %struct.sresult, %struct.sresult* %6, i32 0, i32 2, !dbg !146
  %89 = load i8*, i8** %88, align 8, !dbg !146
  %90 = load i8*, i8** %4, align 8, !dbg !148
  %91 = call i32 @strcmp(i8* %89, i8* %90), !dbg !149
  %92 = icmp eq i32 %91, 0, !dbg !150
  br i1 %92, label %93, label %94, !dbg !151

93:                                               ; preds = %84
  store i32 1, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @back, i32 0, i32 4), align 4, !dbg !152
  br label %99, !dbg !153

94:                                               ; preds = %84
  %95 = load i32, i32* %5, align 4, !dbg !154
  %96 = icmp ne i32 %95, 0, !dbg !154
  br i1 %96, label %97, label %98, !dbg !156

97:                                               ; preds = %94
  store i32 1, i32* @fst, align 4, !dbg !157
  br label %98, !dbg !158

98:                                               ; preds = %97, %94
  br label %99

99:                                               ; preds = %98, %93
  br label %100

100:                                              ; preds = %26, %32, %99, %50
  ret void, !dbg !159
}

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i1 immarg) #2

declare void @__nlex_generated_postag(...) #3

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #2

declare void @__nlex_root(%struct.sresult*) #3

declare i8* @strdup(i8*) #3

declare i32 @free(...) #3

declare i32 @strcmp(i8*, i8*) #3

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @main() #0 !dbg !160 {
  %1 = alloca i32, align 4
  %2 = alloca %struct.sresult, align 8
  %3 = alloca i64, align 8
  %4 = alloca i8*, align 8
  %5 = alloca i8*, align 8
  %6 = alloca i64, align 8
  store i32 0, i32* %1, align 4
  call void @llvm.dbg.declare(metadata %struct.sresult* %2, metadata !163, metadata !DIExpression()), !dbg !164
  %7 = bitcast %struct.sresult* %2 to i8*, !dbg !164
  call void @llvm.memset.p0i8.i64(i8* align 8 %7, i8 0, i64 48, i1 false), !dbg !164
  call void @llvm.dbg.declare(metadata i64* %3, metadata !165, metadata !DIExpression()), !dbg !169
  store i64 1024, i64* %3, align 8, !dbg !169
  call void @llvm.dbg.declare(metadata i8** %4, metadata !170, metadata !DIExpression()), !dbg !172
  %8 = load i64, i64* %3, align 8, !dbg !173
  %9 = call i8* @malloc(i64 %8), !dbg !174
  store i8* %9, i8** %4, align 8, !dbg !172
  %10 = load i8*, i8** %4, align 8, !dbg !175
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.1, i64 0, i64 0), %struct.sresult* %2, i8* %10), !dbg !176
  br label %12, !dbg !177

12:                                               ; preds = %0, %68
  call void @llvm.dbg.declare(metadata i8** %5, metadata !178, metadata !DIExpression()), !dbg !180
  store i8* inttoptr (i64 -1 to i8*), i8** %5, align 8, !dbg !180
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.2, i64 0, i64 0)), !dbg !181
  call void @llvm.dbg.declare(metadata i64* %6, metadata !182, metadata !DIExpression()), !dbg !183
  %14 = load %struct._IO_FILE*, %struct._IO_FILE** @stdin, align 8, !dbg !184
  %15 = call i64 @getline(i8** %4, i64* %3, %struct._IO_FILE* %14), !dbg !185
  store i64 %15, i64* %6, align 8, !dbg !183
  %16 = load i8*, i8** %4, align 8, !dbg !186
  %17 = load i64, i64* %6, align 8, !dbg !187
  %18 = sub i64 %17, 1, !dbg !188
  %19 = getelementptr inbounds i8, i8* %16, i64 %18, !dbg !186
  store i8 0, i8* %19, align 1, !dbg !189
  %20 = load i8*, i8** %4, align 8, !dbg !190
  %21 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.3, i64 0, i64 0), i8* %20), !dbg !191
  call void @__nlex_reset_state(), !dbg !192
  %22 = load i8*, i8** %4, align 8, !dbg !193
  call void @__nlex_feed(i8* %22), !dbg !194
  br label %23, !dbg !195

23:                                               ; preds = %65, %12
  %24 = call i32 @__nlex_can_produce_token(), !dbg !196
  %25 = icmp ne i32 %24, 0, !dbg !195
  br i1 %25, label %26, label %68, !dbg !195

26:                                               ; preds = %23
  call void @__nlex_postag(%struct.sresult* %2, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @.str.4, i64 0, i64 0)), !dbg !197
  %27 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 3, !dbg !199
  %28 = load i8, i8* %27, align 8, !dbg !199
  %29 = sext i8 %28 to i32, !dbg !200
  %30 = icmp ne i32 %29, 0, !dbg !200
  %31 = zext i1 %30 to i64, !dbg !200
  %32 = select i1 %30, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.6, i64 0, i64 0), i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str, i64 0, i64 0), !dbg !200
  %33 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 1, !dbg !201
  %34 = load i32, i32* %33, align 8, !dbg !201
  %35 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 0, !dbg !202
  %36 = load i8*, i8** %35, align 8, !dbg !202
  %37 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 5, !dbg !203
  %38 = load i8*, i8** %37, align 8, !dbg !203
  %39 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 1, !dbg !204
  %40 = load i32, i32* %39, align 8, !dbg !204
  %41 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 2, !dbg !205
  %42 = load i8*, i8** %41, align 8, !dbg !205
  %43 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 4, !dbg !206
  %44 = load i8, i8* %43, align 1, !dbg !206
  %45 = zext i8 %44 to i32, !dbg !207
  %46 = and i32 %45, 1, !dbg !208
  %47 = icmp ne i32 %46, 0, !dbg !207
  %48 = zext i1 %47 to i64, !dbg !207
  %49 = select i1 %47, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.7, i64 0, i64 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.8, i64 0, i64 0), !dbg !207
  %50 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([46 x i8], [46 x i8]* @.str.5, i64 0, i64 0), i8* %32, i32 %34, i8* %36, i8* %38, i32 %40, i8* %42, i8* %49), !dbg !209
  %51 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 3, !dbg !210
  %52 = load i8, i8* %51, align 8, !dbg !210
  %53 = sext i8 %52 to i32, !dbg !212
  %54 = icmp ne i32 %53, 0, !dbg !212
  br i1 %54, label %64, label %55, !dbg !213

55:                                               ; preds = %26
  %56 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 1, !dbg !214
  %57 = load i32, i32* %56, align 8, !dbg !214
  %58 = icmp eq i32 %57, 0, !dbg !215
  br i1 %58, label %64, label %59, !dbg !216

59:                                               ; preds = %55
  %60 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 0, !dbg !217
  %61 = load i8*, i8** %60, align 8, !dbg !217
  %62 = load i8*, i8** %5, align 8, !dbg !218
  %63 = icmp eq i8* %61, %62, !dbg !219
  br i1 %63, label %64, label %65, !dbg !220

64:                                               ; preds = %59, %55, %26
  br label %68, !dbg !221

65:                                               ; preds = %59
  %66 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 0, !dbg !222
  %67 = load i8*, i8** %66, align 8, !dbg !222
  store i8* %67, i8** %5, align 8, !dbg !223
  br label %23, !dbg !195, !llvm.loop !224

68:                                               ; preds = %64, %23
  br label %12, !dbg !177, !llvm.loop !226
}

declare i8* @malloc(i64) #3

declare i32 @printf(i8*, ...) #3

declare i64 @getline(i8**, i64*, %struct._IO_FILE*) #3

declare void @__nlex_feed(i8*) #3

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal void @__nlex_postag(%struct.sresult*, i8*) #0 !dbg !228 {
  %3 = alloca %struct.sresult*, align 8
  %4 = alloca i8*, align 8
  store %struct.sresult* %0, %struct.sresult** %3, align 8
  call void @llvm.dbg.declare(metadata %struct.sresult** %3, metadata !229, metadata !DIExpression()), !dbg !230
  store i8* %1, i8** %4, align 8
  call void @llvm.dbg.declare(metadata i8** %4, metadata !231, metadata !DIExpression()), !dbg !232
  %5 = load i32, i32* @fst, align 4, !dbg !233
  %6 = icmp ne i32 %5, 0, !dbg !233
  br i1 %6, label %7, label %35, !dbg !235

7:                                                ; preds = %2
  br label %8, !dbg !236

8:                                                ; preds = %17, %7
  %9 = load i32, i32* @done, align 4, !dbg !238
  %10 = icmp ne i32 %9, 0, !dbg !238
  br i1 %10, label %15, label %11, !dbg !239

11:                                               ; preds = %8
  %12 = load i32, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @back, i32 0, i32 4), align 4, !dbg !240
  %13 = icmp ne i32 %12, 0, !dbg !241
  %14 = xor i1 %13, true, !dbg !241
  br label %15

15:                                               ; preds = %11, %8
  %16 = phi i1 [ false, %8 ], [ %14, %11 ], !dbg !242
  br i1 %16, label %17, label %20, !dbg !236

17:                                               ; preds = %15
  %18 = load %struct.sresult*, %struct.sresult** %3, align 8, !dbg !243
  %19 = load i8*, i8** %4, align 8, !dbg !244
  call void @__nlex_postag_m(%struct.sresult* %18, i8* %19), !dbg !245
  br label %8, !dbg !236, !llvm.loop !246

20:                                               ; preds = %15
  call void @swap_buffers(), !dbg !248
  call void @resetback(), !dbg !249
  call void (...) @__nlex_generated_postag(), !dbg !250
  %21 = load i32, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @front, i32 0, i32 2), align 4, !dbg !251
  %22 = load i32, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @front, i32 0, i32 3), align 8, !dbg !253
  %23 = icmp slt i32 %21, %22, !dbg !254
  br i1 %23, label %24, label %33, !dbg !255

24:                                               ; preds = %20
  %25 = load %struct.sresult*, %struct.sresult** %3, align 8, !dbg !256
  %26 = load %struct.sresult*, %struct.sresult** getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @front, i32 0, i32 0), align 8, !dbg !257
  %27 = load i32, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @front, i32 0, i32 2), align 4, !dbg !258
  %28 = add nsw i32 %27, 1, !dbg !258
  store i32 %28, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @front, i32 0, i32 2), align 4, !dbg !258
  %29 = sext i32 %27 to i64, !dbg !259
  %30 = getelementptr inbounds %struct.sresult, %struct.sresult* %26, i64 %29, !dbg !259
  %31 = bitcast %struct.sresult* %25 to i8*, !dbg !259
  %32 = bitcast %struct.sresult* %30 to i8*, !dbg !259
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %31, i8* align 8 %32, i64 48, i1 false), !dbg !259
  br label %34, !dbg !260

33:                                               ; preds = %20
  store i32 0, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @front, i32 0, i32 4), align 4, !dbg !261
  br label %34

34:                                               ; preds = %33, %24
  store i32 0, i32* @fst, align 4, !dbg !263
  br label %38, !dbg !264

35:                                               ; preds = %2
  %36 = load %struct.sresult*, %struct.sresult** %3, align 8, !dbg !265
  %37 = load i8*, i8** %4, align 8, !dbg !266
  call void @__nlex_postag_m(%struct.sresult* %36, i8* %37), !dbg !267
  br label %38

38:                                               ; preds = %35, %34
  ret void, !dbg !268
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal void @resetfront() #0 !dbg !269 {
  store i32 0, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @front, i32 0, i32 4), align 4, !dbg !272
  store i32 0, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @front, i32 0, i32 2), align 4, !dbg !273
  store i32 0, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @front, i32 0, i32 3), align 8, !dbg !274
  ret void, !dbg !275
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal void @swap_buffers() #0 !dbg !276 {
  %1 = alloca %struct.WorkingBuffer, align 8
  call void @llvm.dbg.declare(metadata %struct.WorkingBuffer* %1, metadata !277, metadata !DIExpression()), !dbg !278
  %2 = bitcast %struct.WorkingBuffer* %1 to i8*, !dbg !279
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %2, i8* align 8 bitcast (%struct.WorkingBuffer* @back to i8*), i64 24, i1 false), !dbg !279
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 bitcast (%struct.WorkingBuffer* @back to i8*), i8* align 8 bitcast (%struct.WorkingBuffer* @front to i8*), i64 24, i1 false), !dbg !280
  %3 = bitcast %struct.WorkingBuffer* %1 to i8*, !dbg !281
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 bitcast (%struct.WorkingBuffer* @front to i8*), i8* align 8 %3, i64 24, i1 false), !dbg !281
  ret void, !dbg !282
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal void @resetback() #0 !dbg !283 {
  store i32 0, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @back, i32 0, i32 4), align 4, !dbg !284
  store i32 0, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @back, i32 0, i32 2), align 4, !dbg !285
  store i32 0, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @back, i32 0, i32 3), align 8, !dbg !286
  ret void, !dbg !287
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal void @__nlex_reset_state() #0 !dbg !288 {
  call void @resetback(), !dbg !289
  call void @resetfront(), !dbg !290
  store i32 0, i32* @done, align 4, !dbg !291
  store i32 1, i32* @fst, align 4, !dbg !292
  ret void, !dbg !293
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal i32 @__nlex_can_produce_token() #0 !dbg !294 {
  %1 = load i32, i32* @done, align 4, !dbg !295
  %2 = icmp ne i32 %1, 0, !dbg !295
  br i1 %2, label %3, label %7, !dbg !296

3:                                                ; preds = %0
  %4 = load i32, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @front, i32 0, i32 2), align 4, !dbg !297
  %5 = load i32, i32* getelementptr inbounds (%struct.WorkingBuffer, %struct.WorkingBuffer* @front, i32 0, i32 3), align 8, !dbg !298
  %6 = icmp slt i32 %4, %5, !dbg !299
  br label %7, !dbg !296

7:                                                ; preds = %3, %0
  %8 = phi i1 [ true, %0 ], [ %6, %3 ]
  %9 = zext i1 %8 to i32, !dbg !296
  ret i32 %9, !dbg !300
}

attributes #0 = { noinline nounwind optnone sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable }
attributes #2 = { argmemonly nounwind }
attributes #3 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.dbg.cu = !{!2}
!llvm.module.flags = !{!43, !44, !45, !46, !47}
!llvm.ident = !{!48}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "front", scope: !2, file: !3, line: 29, type: !33, isLocal: true, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C99, file: !3, producer: "clang version 9.0.0 (tags/RELEASE_900/final)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !4, globals: !5, nameTableKind: None)
!3 = !DIFile(filename: "test.c", directory: "/home/Test/Documents/nlp-lex/src/playground")
!4 = !{}
!5 = !{!6, !25, !0, !27, !29, !31, !41}
!6 = !DIGlobalVariableExpression(var: !7, expr: !DIExpression())
!7 = distinct !DIGlobalVariable(name: "bufferA", scope: !2, file: !3, line: 16, type: !8, isLocal: false, isDefinition: true)
!8 = !DICompositeType(tag: DW_TAG_array_type, baseType: !9, size: 393216, elements: !23)
!9 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "sresult", file: !3, line: 3, size: 384, elements: !10)
!10 = !{!11, !15, !17, !18, !19, !21, !22}
!11 = !DIDerivedType(tag: DW_TAG_member, name: "start", scope: !9, file: !3, line: 4, baseType: !12, size: 64)
!12 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !13, size: 64)
!13 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !14)
!14 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!15 = !DIDerivedType(tag: DW_TAG_member, name: "length", scope: !9, file: !3, line: 5, baseType: !16, size: 32, offset: 64)
!16 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!17 = !DIDerivedType(tag: DW_TAG_member, name: "tag", scope: !9, file: !3, line: 6, baseType: !12, size: 64, offset: 128)
!18 = !DIDerivedType(tag: DW_TAG_member, name: "errc", scope: !9, file: !3, line: 7, baseType: !14, size: 8, offset: 192)
!19 = !DIDerivedType(tag: DW_TAG_member, name: "metadata", scope: !9, file: !3, line: 8, baseType: !20, size: 8, offset: 200)
!20 = !DIBasicType(name: "unsigned char", size: 8, encoding: DW_ATE_unsigned_char)
!21 = !DIDerivedType(tag: DW_TAG_member, name: "pos", scope: !9, file: !3, line: 9, baseType: !12, size: 64, offset: 256)
!22 = !DIDerivedType(tag: DW_TAG_member, name: "allocd", scope: !9, file: !3, line: 10, baseType: !16, size: 32, offset: 320)
!23 = !{!24}
!24 = !DISubrange(count: 1024)
!25 = !DIGlobalVariableExpression(var: !26, expr: !DIExpression())
!26 = distinct !DIGlobalVariable(name: "bufferB", scope: !2, file: !3, line: 17, type: !8, isLocal: false, isDefinition: true)
!27 = !DIGlobalVariableExpression(var: !28, expr: !DIExpression())
!28 = distinct !DIGlobalVariable(name: "dumpout", scope: !2, file: !3, line: 51, type: !16, isLocal: true, isDefinition: true)
!29 = !DIGlobalVariableExpression(var: !30, expr: !DIExpression())
!30 = distinct !DIGlobalVariable(name: "done", scope: !2, file: !3, line: 49, type: !16, isLocal: true, isDefinition: true)
!31 = !DIGlobalVariableExpression(var: !32, expr: !DIExpression())
!32 = distinct !DIGlobalVariable(name: "back", scope: !2, file: !3, line: 27, type: !33, isLocal: true, isDefinition: true)
!33 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "WorkingBuffer", file: !3, line: 19, size: 192, elements: !34)
!34 = !{!35, !37, !38, !39, !40}
!35 = !DIDerivedType(tag: DW_TAG_member, name: "buffer", scope: !33, file: !3, line: 20, baseType: !36, size: 64)
!36 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !9, size: 64)
!37 = !DIDerivedType(tag: DW_TAG_member, name: "size", scope: !33, file: !3, line: 21, baseType: !16, size: 32, offset: 64)
!38 = !DIDerivedType(tag: DW_TAG_member, name: "start", scope: !33, file: !3, line: 22, baseType: !16, size: 32, offset: 96)
!39 = !DIDerivedType(tag: DW_TAG_member, name: "end", scope: !33, file: !3, line: 23, baseType: !16, size: 32, offset: 128)
!40 = !DIDerivedType(tag: DW_TAG_member, name: "ready", scope: !33, file: !3, line: 24, baseType: !16, size: 32, offset: 160)
!41 = !DIGlobalVariableExpression(var: !42, expr: !DIExpression())
!42 = distinct !DIGlobalVariable(name: "fst", scope: !2, file: !3, line: 50, type: !16, isLocal: true, isDefinition: true)
!43 = !{i32 2, !"Dwarf Version", i32 4}
!44 = !{i32 2, !"Debug Info Version", i32 3}
!45 = !{i32 1, !"wchar_size", i32 4}
!46 = !{i32 7, !"PIC Level", i32 2}
!47 = !{i32 7, !"PIE Level", i32 2}
!48 = !{!"clang version 9.0.0 (tags/RELEASE_900/final)"}
!49 = distinct !DISubprogram(name: "__nlex_postag_m", scope: !3, file: !3, line: 53, type: !50, scopeLine: 53, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!50 = !DISubroutineType(types: !51)
!51 = !{null, !36, !12}
!52 = !DILocalVariable(name: "res", arg: 1, scope: !49, file: !3, line: 53, type: !36)
!53 = !DILocation(line: 53, column: 38, scope: !49)
!54 = !DILocalVariable(name: "ptag", arg: 2, scope: !49, file: !3, line: 53, type: !12)
!55 = !DILocation(line: 53, column: 55, scope: !49)
!56 = !DILocalVariable(name: "pos_fst", scope: !49, file: !3, line: 54, type: !16)
!57 = !DILocation(line: 54, column: 7, scope: !49)
!58 = !DILocation(line: 55, column: 13, scope: !59)
!59 = distinct !DILexicalBlock(scope: !49, file: !3, line: 55, column: 7)
!60 = !DILocation(line: 55, column: 7, scope: !59)
!61 = !DILocation(line: 55, column: 7, scope: !49)
!62 = !DILocation(line: 57, column: 6, scope: !63)
!63 = distinct !DILexicalBlock(scope: !59, file: !3, line: 55, column: 20)
!64 = !DILocation(line: 57, column: 18, scope: !63)
!65 = !DILocation(line: 57, column: 36, scope: !63)
!66 = !DILocation(line: 57, column: 12, scope: !63)
!67 = !DILocation(line: 58, column: 15, scope: !68)
!68 = distinct !DILexicalBlock(scope: !63, file: !3, line: 58, column: 9)
!69 = !DILocation(line: 58, column: 30, scope: !68)
!70 = !DILocation(line: 58, column: 21, scope: !68)
!71 = !DILocation(line: 58, column: 9, scope: !63)
!72 = !DILocation(line: 59, column: 15, scope: !73)
!73 = distinct !DILexicalBlock(scope: !68, file: !3, line: 58, column: 35)
!74 = !DILocation(line: 60, column: 7, scope: !73)
!75 = !DILocation(line: 61, column: 15, scope: !73)
!76 = !DILocation(line: 62, column: 5, scope: !73)
!77 = !DILocation(line: 63, column: 9, scope: !78)
!78 = distinct !DILexicalBlock(scope: !63, file: !3, line: 63, column: 9)
!79 = !DILocation(line: 63, column: 9, scope: !63)
!80 = !DILocation(line: 64, column: 7, scope: !78)
!81 = !DILocation(line: 65, column: 3, scope: !63)
!82 = !DILocation(line: 66, column: 13, scope: !59)
!83 = !DILocation(line: 67, column: 7, scope: !84)
!84 = distinct !DILexicalBlock(scope: !49, file: !3, line: 67, column: 7)
!85 = !DILocation(line: 67, column: 7, scope: !49)
!86 = !DILocation(line: 68, column: 5, scope: !84)
!87 = !DILocation(line: 69, column: 12, scope: !88)
!88 = distinct !DILexicalBlock(scope: !49, file: !3, line: 69, column: 7)
!89 = !DILocation(line: 69, column: 7, scope: !88)
!90 = !DILocation(line: 69, column: 7, scope: !49)
!91 = !DILocation(line: 70, column: 5, scope: !92)
!92 = distinct !DILexicalBlock(scope: !88, file: !3, line: 69, column: 19)
!93 = !DILocation(line: 71, column: 5, scope: !92)
!94 = !DILocation(line: 72, column: 5, scope: !92)
!95 = !DILocation(line: 73, column: 15, scope: !96)
!96 = distinct !DILexicalBlock(scope: !92, file: !3, line: 73, column: 9)
!97 = !DILocation(line: 73, column: 29, scope: !96)
!98 = !DILocation(line: 73, column: 21, scope: !96)
!99 = !DILocation(line: 73, column: 9, scope: !92)
!100 = !DILocation(line: 74, column: 8, scope: !96)
!101 = !DILocation(line: 74, column: 20, scope: !96)
!102 = !DILocation(line: 74, column: 38, scope: !96)
!103 = !DILocation(line: 74, column: 14, scope: !96)
!104 = !DILocation(line: 74, column: 7, scope: !96)
!105 = !DILocation(line: 76, column: 19, scope: !106)
!106 = distinct !DILexicalBlock(scope: !96, file: !3, line: 75, column: 10)
!107 = !DILocation(line: 78, column: 3, scope: !92)
!108 = !DILocalVariable(name: "mres", scope: !109, file: !3, line: 79, type: !9)
!109 = distinct !DILexicalBlock(scope: !88, file: !3, line: 78, column: 10)
!110 = !DILocation(line: 79, column: 20, scope: !109)
!111 = !DILocation(line: 80, column: 5, scope: !109)
!112 = !DILocation(line: 81, column: 14, scope: !113)
!113 = distinct !DILexicalBlock(scope: !109, file: !3, line: 81, column: 9)
!114 = !DILocation(line: 81, column: 9, scope: !113)
!115 = !DILocation(line: 81, column: 19, scope: !113)
!116 = !DILocation(line: 81, column: 27, scope: !113)
!117 = !DILocation(line: 81, column: 34, scope: !113)
!118 = !DILocation(line: 81, column: 9, scope: !109)
!119 = !DILocation(line: 84, column: 12, scope: !120)
!120 = distinct !DILexicalBlock(scope: !113, file: !3, line: 81, column: 40)
!121 = !DILocation(line: 84, column: 18, scope: !120)
!122 = !DILocation(line: 85, column: 12, scope: !120)
!123 = !DILocation(line: 85, column: 19, scope: !120)
!124 = !DILocation(line: 86, column: 12, scope: !120)
!125 = !DILocation(line: 87, column: 5, scope: !120)
!126 = !DILocation(line: 88, column: 32, scope: !113)
!127 = !DILocation(line: 88, column: 20, scope: !113)
!128 = !DILocation(line: 88, column: 12, scope: !113)
!129 = !DILocation(line: 88, column: 18, scope: !113)
!130 = !DILocalVariable(name: "p", scope: !109, file: !3, line: 89, type: !36)
!131 = !DILocation(line: 89, column: 21, scope: !109)
!132 = !DILocation(line: 89, column: 31, scope: !109)
!133 = !DILocation(line: 89, column: 46, scope: !109)
!134 = !DILocation(line: 89, column: 26, scope: !109)
!135 = !DILocation(line: 90, column: 9, scope: !136)
!136 = distinct !DILexicalBlock(scope: !109, file: !3, line: 90, column: 9)
!137 = !DILocation(line: 90, column: 12, scope: !136)
!138 = !DILocation(line: 90, column: 9, scope: !109)
!139 = !DILocation(line: 91, column: 12, scope: !140)
!140 = distinct !DILexicalBlock(scope: !136, file: !3, line: 90, column: 20)
!141 = !DILocation(line: 91, column: 15, scope: !140)
!142 = !DILocation(line: 91, column: 7, scope: !140)
!143 = !DILocation(line: 92, column: 5, scope: !140)
!144 = !DILocation(line: 93, column: 6, scope: !109)
!145 = !DILocation(line: 93, column: 10, scope: !109)
!146 = !DILocation(line: 94, column: 21, scope: !147)
!147 = distinct !DILexicalBlock(scope: !109, file: !3, line: 94, column: 9)
!148 = !DILocation(line: 94, column: 26, scope: !147)
!149 = !DILocation(line: 94, column: 9, scope: !147)
!150 = !DILocation(line: 94, column: 32, scope: !147)
!151 = !DILocation(line: 94, column: 9, scope: !109)
!152 = !DILocation(line: 95, column: 18, scope: !147)
!153 = !DILocation(line: 95, column: 7, scope: !147)
!154 = !DILocation(line: 96, column: 14, scope: !155)
!155 = distinct !DILexicalBlock(scope: !147, file: !3, line: 96, column: 14)
!156 = !DILocation(line: 96, column: 14, scope: !147)
!157 = !DILocation(line: 97, column: 11, scope: !155)
!158 = !DILocation(line: 97, column: 7, scope: !155)
!159 = !DILocation(line: 99, column: 1, scope: !49)
!160 = distinct !DISubprogram(name: "main", scope: !3, file: !3, line: 130, type: !161, scopeLine: 130, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!161 = !DISubroutineType(types: !162)
!162 = !{!16}
!163 = !DILocalVariable(name: "res", scope: !160, file: !3, line: 131, type: !9)
!164 = !DILocation(line: 131, column: 18, scope: !160)
!165 = !DILocalVariable(name: "size", scope: !160, file: !3, line: 132, type: !166)
!166 = !DIDerivedType(tag: DW_TAG_typedef, name: "size_t", file: !167, line: 46, baseType: !168)
!167 = !DIFile(filename: "/usr/lib/clang/9.0.0/include/stddef.h", directory: "")
!168 = !DIBasicType(name: "long unsigned int", size: 64, encoding: DW_ATE_unsigned)
!169 = !DILocation(line: 132, column: 10, scope: !160)
!170 = !DILocalVariable(name: "s", scope: !160, file: !3, line: 133, type: !171)
!171 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !14, size: 64)
!172 = !DILocation(line: 133, column: 9, scope: !160)
!173 = !DILocation(line: 133, column: 20, scope: !160)
!174 = !DILocation(line: 133, column: 13, scope: !160)
!175 = !DILocation(line: 134, column: 40, scope: !160)
!176 = !DILocation(line: 134, column: 3, scope: !160)
!177 = !DILocation(line: 135, column: 3, scope: !160)
!178 = !DILocalVariable(name: "last", scope: !179, file: !3, line: 136, type: !171)
!179 = distinct !DILexicalBlock(scope: !160, file: !3, line: 135, column: 13)
!180 = !DILocation(line: 136, column: 11, scope: !179)
!181 = !DILocation(line: 137, column: 5, scope: !179)
!182 = !DILocalVariable(name: "els", scope: !179, file: !3, line: 138, type: !166)
!183 = !DILocation(line: 138, column: 12, scope: !179)
!184 = !DILocation(line: 138, column: 37, scope: !179)
!185 = !DILocation(line: 138, column: 18, scope: !179)
!186 = !DILocation(line: 139, column: 5, scope: !179)
!187 = !DILocation(line: 139, column: 7, scope: !179)
!188 = !DILocation(line: 139, column: 11, scope: !179)
!189 = !DILocation(line: 139, column: 16, scope: !179)
!190 = !DILocation(line: 140, column: 35, scope: !179)
!191 = !DILocation(line: 140, column: 5, scope: !179)
!192 = !DILocation(line: 141, column: 5, scope: !179)
!193 = !DILocation(line: 142, column: 17, scope: !179)
!194 = !DILocation(line: 142, column: 5, scope: !179)
!195 = !DILocation(line: 143, column: 5, scope: !179)
!196 = !DILocation(line: 143, column: 12, scope: !179)
!197 = !DILocation(line: 144, column: 7, scope: !198)
!198 = distinct !DILexicalBlock(scope: !179, file: !3, line: 143, column: 40)
!199 = !DILocation(line: 147, column: 19, scope: !198)
!200 = !DILocation(line: 147, column: 15, scope: !198)
!201 = !DILocation(line: 147, column: 43, scope: !198)
!202 = !DILocation(line: 147, column: 55, scope: !198)
!203 = !DILocation(line: 147, column: 66, scope: !198)
!204 = !DILocation(line: 148, column: 18, scope: !198)
!205 = !DILocation(line: 148, column: 30, scope: !198)
!206 = !DILocation(line: 148, column: 40, scope: !198)
!207 = !DILocation(line: 148, column: 36, scope: !198)
!208 = !DILocation(line: 148, column: 49, scope: !198)
!209 = !DILocation(line: 146, column: 7, scope: !198)
!210 = !DILocation(line: 149, column: 15, scope: !211)
!211 = distinct !DILexicalBlock(scope: !198, file: !3, line: 149, column: 11)
!212 = !DILocation(line: 149, column: 11, scope: !211)
!213 = !DILocation(line: 149, column: 20, scope: !211)
!214 = !DILocation(line: 149, column: 27, scope: !211)
!215 = !DILocation(line: 149, column: 34, scope: !211)
!216 = !DILocation(line: 149, column: 39, scope: !211)
!217 = !DILocation(line: 149, column: 46, scope: !211)
!218 = !DILocation(line: 149, column: 55, scope: !211)
!219 = !DILocation(line: 149, column: 52, scope: !211)
!220 = !DILocation(line: 149, column: 11, scope: !198)
!221 = !DILocation(line: 150, column: 9, scope: !211)
!222 = !DILocation(line: 151, column: 18, scope: !198)
!223 = !DILocation(line: 151, column: 12, scope: !198)
!224 = distinct !{!224, !195, !225}
!225 = !DILocation(line: 152, column: 5, scope: !179)
!226 = distinct !{!226, !177, !227}
!227 = !DILocation(line: 153, column: 3, scope: !160)
!228 = distinct !DISubprogram(name: "__nlex_postag", scope: !3, file: !3, line: 101, type: !50, scopeLine: 101, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !4)
!229 = !DILocalVariable(name: "res", arg: 1, scope: !228, file: !3, line: 101, type: !36)
!230 = !DILocation(line: 101, column: 43, scope: !228)
!231 = !DILocalVariable(name: "ptag", arg: 2, scope: !228, file: !3, line: 101, type: !12)
!232 = !DILocation(line: 101, column: 60, scope: !228)
!233 = !DILocation(line: 102, column: 7, scope: !234)
!234 = distinct !DILexicalBlock(scope: !228, file: !3, line: 102, column: 7)
!235 = !DILocation(line: 102, column: 7, scope: !228)
!236 = !DILocation(line: 103, column: 5, scope: !237)
!237 = distinct !DILexicalBlock(scope: !234, file: !3, line: 102, column: 12)
!238 = !DILocation(line: 103, column: 13, scope: !237)
!239 = !DILocation(line: 103, column: 18, scope: !237)
!240 = !DILocation(line: 103, column: 27, scope: !237)
!241 = !DILocation(line: 103, column: 21, scope: !237)
!242 = !DILocation(line: 0, scope: !237)
!243 = !DILocation(line: 104, column: 23, scope: !237)
!244 = !DILocation(line: 104, column: 28, scope: !237)
!245 = !DILocation(line: 104, column: 7, scope: !237)
!246 = distinct !{!246, !236, !247}
!247 = !DILocation(line: 104, column: 32, scope: !237)
!248 = !DILocation(line: 105, column: 5, scope: !237)
!249 = !DILocation(line: 106, column: 5, scope: !237)
!250 = !DILocation(line: 107, column: 5, scope: !237)
!251 = !DILocation(line: 108, column: 15, scope: !252)
!252 = distinct !DILexicalBlock(scope: !237, file: !3, line: 108, column: 9)
!253 = !DILocation(line: 108, column: 29, scope: !252)
!254 = !DILocation(line: 108, column: 21, scope: !252)
!255 = !DILocation(line: 108, column: 9, scope: !237)
!256 = !DILocation(line: 109, column: 8, scope: !252)
!257 = !DILocation(line: 109, column: 20, scope: !252)
!258 = !DILocation(line: 109, column: 38, scope: !252)
!259 = !DILocation(line: 109, column: 14, scope: !252)
!260 = !DILocation(line: 109, column: 7, scope: !252)
!261 = !DILocation(line: 111, column: 19, scope: !262)
!262 = distinct !DILexicalBlock(scope: !252, file: !3, line: 110, column: 10)
!263 = !DILocation(line: 113, column: 9, scope: !237)
!264 = !DILocation(line: 114, column: 3, scope: !237)
!265 = !DILocation(line: 115, column: 21, scope: !234)
!266 = !DILocation(line: 115, column: 26, scope: !234)
!267 = !DILocation(line: 115, column: 5, scope: !234)
!268 = !DILocation(line: 116, column: 1, scope: !228)
!269 = distinct !DISubprogram(name: "resetfront", scope: !3, file: !3, line: 37, type: !270, scopeLine: 37, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !4)
!270 = !DISubroutineType(types: !271)
!271 = !{null}
!272 = !DILocation(line: 38, column: 15, scope: !269)
!273 = !DILocation(line: 39, column: 15, scope: !269)
!274 = !DILocation(line: 40, column: 13, scope: !269)
!275 = !DILocation(line: 41, column: 1, scope: !269)
!276 = distinct !DISubprogram(name: "swap_buffers", scope: !3, file: !3, line: 31, type: !270, scopeLine: 31, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !4)
!277 = !DILocalVariable(name: "buf", scope: !276, file: !3, line: 32, type: !33)
!278 = !DILocation(line: 32, column: 24, scope: !276)
!279 = !DILocation(line: 32, column: 30, scope: !276)
!280 = !DILocation(line: 33, column: 10, scope: !276)
!281 = !DILocation(line: 34, column: 11, scope: !276)
!282 = !DILocation(line: 35, column: 1, scope: !276)
!283 = distinct !DISubprogram(name: "resetback", scope: !3, file: !3, line: 43, type: !270, scopeLine: 43, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !4)
!284 = !DILocation(line: 44, column: 14, scope: !283)
!285 = !DILocation(line: 45, column: 14, scope: !283)
!286 = !DILocation(line: 46, column: 12, scope: !283)
!287 = !DILocation(line: 47, column: 1, scope: !283)
!288 = distinct !DISubprogram(name: "__nlex_reset_state", scope: !3, file: !3, line: 122, type: !270, scopeLine: 122, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !4)
!289 = !DILocation(line: 123, column: 3, scope: !288)
!290 = !DILocation(line: 124, column: 3, scope: !288)
!291 = !DILocation(line: 125, column: 8, scope: !288)
!292 = !DILocation(line: 126, column: 7, scope: !288)
!293 = !DILocation(line: 127, column: 1, scope: !288)
!294 = distinct !DISubprogram(name: "__nlex_can_produce_token", scope: !3, file: !3, line: 118, type: !161, scopeLine: 118, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !4)
!295 = !DILocation(line: 119, column: 11, scope: !294)
!296 = !DILocation(line: 119, column: 16, scope: !294)
!297 = !DILocation(line: 119, column: 25, scope: !294)
!298 = !DILocation(line: 119, column: 39, scope: !294)
!299 = !DILocation(line: 119, column: 31, scope: !294)
!300 = !DILocation(line: 119, column: 3, scope: !294)
