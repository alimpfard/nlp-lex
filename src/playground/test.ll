; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i64, i16, i8, [1 x i8], i8*, i64, %struct._IO_codecvt*, %struct._IO_wide_data*, %struct._IO_FILE*, i8*, i64, i32, [20 x i8] }
%struct._IO_marker = type opaque
%struct._IO_codecvt = type opaque
%struct._IO_wide_data = type opaque
%struct.__va_list_tag = type { i32, i32, i8*, i8* }
%struct.sresult = type { i8*, i32, i8*, i8, i8, i8*, i32 }

@stderr = external local_unnamed_addr global %struct._IO_FILE*, align 8
@.str = private unnamed_addr constant [3 x i8] c"%c\00", align 1
@.str.1 = private unnamed_addr constant [4 x i8] c"%lf\00", align 1
@.str.2 = private unnamed_addr constant [20 x i8] c"res at %p, s at %p\0A\00", align 1
@.str.3 = private unnamed_addr constant [3 x i8] c"> \00", align 1
@stdin = external local_unnamed_addr global %struct._IO_FILE*, align 8
@.str.5 = private unnamed_addr constant [19 x i8] c"processing - '%s'\0A\00", align 1
@.str.6 = private unnamed_addr constant [46 x i8] c"%smatch {'%.*s' - %s - %d %s} is%sa stopword\0A\00", align 1
@.str.7 = private unnamed_addr constant [4 x i8] c"no \00", align 1
@.str.8 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@.str.9 = private unnamed_addr constant [2 x i8] c" \00", align 1
@.str.10 = private unnamed_addr constant [6 x i8] c" not \00", align 1

; Function Attrs: nofree nounwind sspstrong uwtable
define double @putchard(double) local_unnamed_addr #0 !dbg !21 {
  call void @llvm.dbg.value(metadata double %0, metadata !25, metadata !DIExpression()), !dbg !28
  %2 = fptosi double %0 to i8, !dbg !29
  call void @llvm.dbg.value(metadata i8 %2, metadata !26, metadata !DIExpression()), !dbg !28
  %3 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 8, !dbg !30, !tbaa !31
  %4 = sext i8 %2 to i32, !dbg !35
  %5 = tail call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %3, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i64 0, i64 0), i32 %4) #12, !dbg !36
  call void @llvm.dbg.value(metadata i32 %5, metadata !27, metadata !DIExpression()), !dbg !28
  %6 = sitofp i32 %5 to double, !dbg !37
  ret double %6, !dbg !38
}

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #2

; Function Attrs: nofree nounwind
declare i32 @fprintf(%struct._IO_FILE* nocapture, i8* nocapture readonly, ...) local_unnamed_addr #3

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture) #2

; Function Attrs: nofree nounwind sspstrong uwtable
define double @eputchard(double) local_unnamed_addr #0 !dbg !39 {
  call void @llvm.dbg.value(metadata double %0, metadata !41, metadata !DIExpression()), !dbg !44
  %2 = fptosi double %0 to i8, !dbg !45
  call void @llvm.dbg.value(metadata i8 %2, metadata !42, metadata !DIExpression()), !dbg !44
  %3 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 8, !dbg !46, !tbaa !31
  %4 = sext i8 %2 to i32, !dbg !47
  %5 = tail call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %3, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i64 0, i64 0), i32 %4) #12, !dbg !48
  call void @llvm.dbg.value(metadata i32 %5, metadata !43, metadata !DIExpression()), !dbg !44
  %6 = sitofp i32 %5 to double, !dbg !49
  ret double %6, !dbg !50
}

; Function Attrs: nofree nounwind sspstrong uwtable
define double @printd(double) local_unnamed_addr #0 !dbg !51 {
  call void @llvm.dbg.value(metadata double %0, metadata !53, metadata !DIExpression()), !dbg !54
  %2 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i64 0, i64 0), double %0), !dbg !55
  %3 = sitofp i32 %2 to double, !dbg !56
  ret double %3, !dbg !57
}

; Function Attrs: nofree nounwind
declare i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #3

; Function Attrs: nofree nounwind sspstrong uwtable
define double @eprintd(double) local_unnamed_addr #0 !dbg !58 {
  call void @llvm.dbg.value(metadata double %0, metadata !60, metadata !DIExpression()), !dbg !61
  %2 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 8, !dbg !62, !tbaa !31
  %3 = tail call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %2, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i64 0, i64 0), double %0) #12, !dbg !63
  %4 = sitofp i32 %3 to double, !dbg !64
  ret double %4, !dbg !65
}

; Function Attrs: nounwind sspstrong uwtable
define double @dprintdf(double, double, ...) local_unnamed_addr #4 !dbg !66 {
  %3 = alloca [1 x %struct.__va_list_tag], align 16
  call void @llvm.dbg.value(metadata double %0, metadata !70, metadata !DIExpression()), !dbg !90
  call void @llvm.dbg.value(metadata double %1, metadata !71, metadata !DIExpression()), !dbg !90
  %4 = bitcast [1 x %struct.__va_list_tag]* %3 to i8*, !dbg !91
  call void @llvm.lifetime.start.p0i8(i64 24, i8* nonnull %4) #5, !dbg !91
  call void @llvm.dbg.declare(metadata [1 x %struct.__va_list_tag]* %3, metadata !72, metadata !DIExpression()), !dbg !92
  %5 = getelementptr inbounds [1 x %struct.__va_list_tag], [1 x %struct.__va_list_tag]* %3, i64 0, i64 0, !dbg !93
  call void @llvm.va_start(i8* nonnull %4), !dbg !93
  %6 = fptosi double %1 to i64, !dbg !94
  call void @llvm.dbg.value(metadata i64 %6, metadata !88, metadata !DIExpression()), !dbg !90
  %7 = fptosi double %0 to i32, !dbg !95
  %8 = inttoptr i64 %6 to i8*, !dbg !96
  %9 = call i32 @vdprintf(i32 %7, i8* %8, %struct.__va_list_tag* nonnull %5) #5, !dbg !97
  %10 = sitofp i32 %9 to double, !dbg !98
  call void @llvm.dbg.value(metadata double %10, metadata !89, metadata !DIExpression()), !dbg !90
  call void @llvm.va_end(i8* nonnull %4), !dbg !99
  call void @llvm.lifetime.end.p0i8(i64 24, i8* nonnull %4) #5, !dbg !100
  ret double %10, !dbg !101
}

; Function Attrs: nounwind
declare void @llvm.va_start(i8*) #5

declare i32 @vdprintf(i32, i8*, %struct.__va_list_tag*) local_unnamed_addr #6

; Function Attrs: nounwind
declare void @llvm.va_end(i8*) #5

; Function Attrs: nofree nounwind sspstrong uwtable
define double @mallocd(double) local_unnamed_addr #0 !dbg !102 {
  call void @llvm.dbg.value(metadata double %0, metadata !104, metadata !DIExpression()), !dbg !108
  %2 = fptoui double %0 to i64, !dbg !109
  call void @llvm.dbg.value(metadata i64 %2, metadata !105, metadata !DIExpression()), !dbg !108
  %3 = tail call noalias i8* @malloc(i64 %2) #5, !dbg !110
  call void @llvm.dbg.value(metadata i8* %3, metadata !106, metadata !DIExpression()), !dbg !108
  %4 = ptrtoint i8* %3 to i64, !dbg !111
  call void @llvm.dbg.value(metadata i64 %4, metadata !107, metadata !DIExpression()), !dbg !108
  %5 = sitofp i64 %4 to double, !dbg !112
  ret double %5, !dbg !113
}

; Function Attrs: nofree nounwind
declare noalias i8* @malloc(i64) local_unnamed_addr #3

; Function Attrs: nounwind sspstrong uwtable
define double @memsetd(double, double, double) local_unnamed_addr #4 !dbg !114 {
  call void @llvm.dbg.value(metadata double %0, metadata !118, metadata !DIExpression()), !dbg !122
  call void @llvm.dbg.value(metadata double %1, metadata !119, metadata !DIExpression()), !dbg !122
  call void @llvm.dbg.value(metadata double %2, metadata !120, metadata !DIExpression()), !dbg !122
  %4 = fptosi double %0 to i64, !dbg !123
  call void @llvm.dbg.value(metadata i64 %4, metadata !121, metadata !DIExpression()), !dbg !122
  %5 = inttoptr i64 %4 to i8*, !dbg !124
  %6 = fptosi double %1 to i32, !dbg !125
  %7 = trunc i32 %6 to i8, !dbg !126
  %8 = fptoui double %2 to i64, !dbg !127
  tail call void @llvm.memset.p0i8.i64(i8* align 1 %5, i8 %7, i64 %8, i1 false), !dbg !126
  call void @llvm.dbg.value(metadata i64 %4, metadata !121, metadata !DIExpression()), !dbg !122
  %9 = sitofp i64 %4 to double, !dbg !128
  ret double %9, !dbg !129
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #2

; Function Attrs: nounwind sspstrong uwtable
define double @freed(double) local_unnamed_addr #4 !dbg !130 {
  call void @llvm.dbg.value(metadata double %0, metadata !132, metadata !DIExpression()), !dbg !134
  %2 = fptosi double %0 to i64, !dbg !135
  call void @llvm.dbg.value(metadata i64 %2, metadata !133, metadata !DIExpression()), !dbg !134
  %3 = inttoptr i64 %2 to i8*, !dbg !136
  tail call void @free(i8* %3) #5, !dbg !137
  ret double 0.000000e+00, !dbg !138
}

; Function Attrs: nounwind
declare void @free(i8* nocapture) local_unnamed_addr #7

; Function Attrs: norecurse nounwind readonly sspstrong uwtable
define double @cderef(double) local_unnamed_addr #8 !dbg !139 {
  call void @llvm.dbg.value(metadata double %0, metadata !141, metadata !DIExpression()), !dbg !143
  %2 = fptosi double %0 to i64, !dbg !144
  call void @llvm.dbg.value(metadata i64 %2, metadata !142, metadata !DIExpression()), !dbg !143
  %3 = inttoptr i64 %2 to i8*, !dbg !145
  %4 = load i8, i8* %3, align 1, !dbg !146, !tbaa !147
  %5 = sitofp i8 %4 to double, !dbg !146
  ret double %5, !dbg !148
}

; Function Attrs: nofree norecurse nounwind sspstrong uwtable writeonly
define double @cderefset(double, double) local_unnamed_addr #9 !dbg !149 {
  call void @llvm.dbg.value(metadata double %0, metadata !153, metadata !DIExpression()), !dbg !156
  call void @llvm.dbg.value(metadata double %1, metadata !154, metadata !DIExpression()), !dbg !156
  %3 = fptosi double %0 to i64, !dbg !157
  call void @llvm.dbg.value(metadata i64 %3, metadata !155, metadata !DIExpression()), !dbg !156
  %4 = fptosi double %1 to i8, !dbg !158
  %5 = inttoptr i64 %3 to i8*, !dbg !159
  store i8 %4, i8* %5, align 1, !dbg !160, !tbaa !147
  %6 = sitofp i8 %4 to double, !dbg !161
  ret double %6, !dbg !162
}

; Function Attrs: norecurse nounwind readonly sspstrong uwtable
define double @dderef(double) local_unnamed_addr #8 !dbg !163 {
  call void @llvm.dbg.value(metadata double %0, metadata !165, metadata !DIExpression()), !dbg !167
  %2 = fptosi double %0 to i64, !dbg !168
  call void @llvm.dbg.value(metadata i64 %2, metadata !166, metadata !DIExpression()), !dbg !167
  %3 = inttoptr i64 %2 to double*, !dbg !169
  %4 = load double, double* %3, align 8, !dbg !170, !tbaa !171
  ret double %4, !dbg !173
}

; Function Attrs: nofree norecurse nounwind sspstrong uwtable writeonly
define double @dderefset(double, double returned) local_unnamed_addr #9 !dbg !174 {
  call void @llvm.dbg.value(metadata double %0, metadata !176, metadata !DIExpression()), !dbg !179
  call void @llvm.dbg.value(metadata double %1, metadata !177, metadata !DIExpression()), !dbg !179
  %3 = fptosi double %0 to i64, !dbg !180
  call void @llvm.dbg.value(metadata i64 %3, metadata !178, metadata !DIExpression()), !dbg !179
  %4 = inttoptr i64 %3 to double*, !dbg !181
  store double %1, double* %4, align 8, !dbg !182, !tbaa !171
  ret double %1, !dbg !183
}

; Function Attrs: nounwind sspstrong uwtable
define double @trap() local_unnamed_addr #4 !dbg !184 {
  tail call void asm sideeffect "int3", "~{dirflag},~{fpsr},~{flags}"() #5, !dbg !187, !srcloc !188
  ret double 0.000000e+00, !dbg !189
}

; Function Attrs: noreturn nounwind sspstrong uwtable
define i32 @main() local_unnamed_addr #10 !dbg !190 {
  %1 = alloca %struct.sresult, align 8
  %2 = alloca i64, align 8
  %3 = alloca i8*, align 8
  %4 = bitcast %struct.sresult* %1 to i8*, !dbg !214
  call void @llvm.lifetime.start.p0i8(i64 48, i8* nonnull %4) #5, !dbg !214
  call void @llvm.memset.p0i8.i64(i8* nonnull align 8 %4, i8 0, i64 48, i1 false), !dbg !215
  %5 = bitcast i64* %2 to i8*, !dbg !216
  call void @llvm.lifetime.start.p0i8(i64 8, i8* nonnull %5) #5, !dbg !216
  call void @llvm.dbg.value(metadata i64 1024000, metadata !207, metadata !DIExpression()), !dbg !217
  store i64 1024000, i64* %2, align 8, !dbg !218, !tbaa !219
  %6 = bitcast i8** %3 to i8*, !dbg !221
  call void @llvm.lifetime.start.p0i8(i64 8, i8* nonnull %6) #5, !dbg !221
  call void @llvm.dbg.value(metadata i64 1024000, metadata !207, metadata !DIExpression()), !dbg !217
  %7 = tail call noalias i8* @malloc(i64 1024000) #5, !dbg !222
  call void @llvm.dbg.value(metadata i8* %7, metadata !208, metadata !DIExpression()), !dbg !217
  store i8* %7, i8** %3, align 8, !dbg !223, !tbaa !31
  call void @llvm.dbg.value(metadata %struct.sresult* %1, metadata !194, metadata !DIExpression(DW_OP_deref)), !dbg !217
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.2, i64 0, i64 0), %struct.sresult* nonnull %1, i8* %7), !dbg !224
  %9 = getelementptr inbounds %struct.sresult, %struct.sresult* %1, i64 0, i32 3, !dbg !225
  %10 = getelementptr inbounds %struct.sresult, %struct.sresult* %1, i64 0, i32 1, !dbg !225
  %11 = getelementptr inbounds %struct.sresult, %struct.sresult* %1, i64 0, i32 0, !dbg !225
  %12 = getelementptr inbounds %struct.sresult, %struct.sresult* %1, i64 0, i32 5, !dbg !225
  %13 = getelementptr inbounds %struct.sresult, %struct.sresult* %1, i64 0, i32 2, !dbg !225
  %14 = getelementptr inbounds %struct.sresult, %struct.sresult* %1, i64 0, i32 4, !dbg !225
  br label %15, !dbg !226

15:                                               ; preds = %26, %0
  call void @llvm.dbg.value(metadata i32 -1, metadata !209, metadata !DIExpression()), !dbg !227
  call void @llvm.dbg.value(metadata i64 0, metadata !211, metadata !DIExpression()), !dbg !227
  %16 = call i32 @isatty(i32 0) #5, !dbg !228
  %17 = icmp eq i32 %16, 0, !dbg !228
  br i1 %17, label %22, label %18, !dbg !230

18:                                               ; preds = %15
  %19 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.3, i64 0, i64 0)), !dbg !231
  %20 = load %struct._IO_FILE*, %struct._IO_FILE** @stdin, align 8, !dbg !233, !tbaa !31
  call void @llvm.dbg.value(metadata i64* %2, metadata !207, metadata !DIExpression(DW_OP_deref)), !dbg !217
  call void @llvm.dbg.value(metadata i8** %3, metadata !208, metadata !DIExpression(DW_OP_deref)), !dbg !217
  %21 = call i64 @getline(i8** nonnull %3, i64* nonnull %2, %struct._IO_FILE* %20) #5, !dbg !234
  call void @llvm.dbg.value(metadata i64 %21, metadata !211, metadata !DIExpression()), !dbg !227
  br label %27, !dbg !235

22:                                               ; preds = %15
  %23 = load i8*, i8** %3, align 8, !dbg !236, !tbaa !31
  call void @llvm.dbg.value(metadata i8* %23, metadata !208, metadata !DIExpression()), !dbg !217
  %24 = call i64 @read(i32 0, i8* %23, i64 1024000) #5, !dbg !238
  call void @llvm.dbg.value(metadata i64 %24, metadata !211, metadata !DIExpression()), !dbg !227
  %25 = icmp eq i64 %24, 0, !dbg !239
  br i1 %25, label %26, label %27, !dbg !241

26:                                               ; preds = %35, %22
  br label %15, !dbg !227, !llvm.loop !242

27:                                               ; preds = %22, %18
  %28 = phi i64 [ %21, %18 ], [ %24, %22 ], !dbg !244
  call void @llvm.dbg.value(metadata i64 %28, metadata !211, metadata !DIExpression()), !dbg !227
  %29 = load i8*, i8** %3, align 8, !dbg !245, !tbaa !31
  call void @llvm.dbg.value(metadata i8* %29, metadata !208, metadata !DIExpression()), !dbg !217
  %30 = add i64 %28, -1, !dbg !246
  %31 = getelementptr inbounds i8, i8* %29, i64 %30, !dbg !245
  store i8 0, i8* %31, align 1, !dbg !247, !tbaa !147
  %32 = load i8*, i8** %3, align 8, !dbg !248, !tbaa !31
  call void @llvm.dbg.value(metadata i8* %32, metadata !208, metadata !DIExpression()), !dbg !217
  %33 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.5, i64 0, i64 0), i8* %32), !dbg !249
  %34 = load i8*, i8** %3, align 8, !dbg !250, !tbaa !31
  call void @llvm.dbg.value(metadata i8* %34, metadata !208, metadata !DIExpression()), !dbg !217
  call void @__nlex_feed(i8* %34) #5, !dbg !251
  br label %35, !dbg !252

35:                                               ; preds = %35, %27
  %36 = phi i32 [ -1, %27 ], [ %49, %35 ], !dbg !253
  call void @llvm.dbg.value(metadata i32 %36, metadata !209, metadata !DIExpression()), !dbg !227
  call void @llvm.dbg.value(metadata %struct.sresult* %1, metadata !194, metadata !DIExpression(DW_OP_deref)), !dbg !217
  call void @__nlex_root(%struct.sresult* nonnull %1) #5, !dbg !254
  %37 = load i8, i8* %9, align 8, !dbg !255, !tbaa !256
  %38 = icmp eq i8 %37, 0, !dbg !259
  %39 = select i1 %38, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str.8, i64 0, i64 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.7, i64 0, i64 0), !dbg !259
  %40 = load i32, i32* %10, align 8, !dbg !260, !tbaa !261
  %41 = load i8*, i8** %11, align 8, !dbg !262, !tbaa !263
  %42 = load i8*, i8** %12, align 8, !dbg !264, !tbaa !265
  %43 = load i8*, i8** %13, align 8, !dbg !266, !tbaa !267
  %44 = load i8, i8* %14, align 1, !dbg !268, !tbaa !269
  %45 = and i8 %44, 1, !dbg !270
  %46 = icmp eq i8 %45, 0, !dbg !271
  %47 = select i1 %46, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.10, i64 0, i64 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.9, i64 0, i64 0), !dbg !271
  %48 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([46 x i8], [46 x i8]* @.str.6, i64 0, i64 0), i8* %39, i32 %40, i8* %41, i8* %42, i32 %40, i8* %43, i8* %47), !dbg !272
  %49 = call i32 (...) @__nlex_distance() #5, !dbg !273
  call void @llvm.dbg.value(metadata i32 %49, metadata !212, metadata !DIExpression()), !dbg !225
  %50 = load i8, i8* %9, align 8, !dbg !274, !tbaa !256
  %51 = icmp ne i8 %50, 0, !dbg !276
  %52 = load i32, i32* %10, align 8, !dbg !277
  %53 = icmp eq i32 %52, 0, !dbg !278
  %54 = or i1 %51, %53, !dbg !279
  %55 = icmp eq i32 %49, %36, !dbg !280
  %56 = or i1 %55, %54, !dbg !279
  br i1 %56, label %26, label %35, !llvm.loop !281
}

; Function Attrs: nounwind
declare i32 @isatty(i32) local_unnamed_addr #7

declare i64 @getline(i8**, i64*, %struct._IO_FILE*) local_unnamed_addr #6

; Function Attrs: nofree
declare i64 @read(i32, i8* nocapture, i64) local_unnamed_addr #11

declare void @__nlex_feed(i8*) local_unnamed_addr #6

declare void @__nlex_root(%struct.sresult*) local_unnamed_addr #6

declare i32 @__nlex_distance(...) local_unnamed_addr #6

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.value(metadata, metadata, metadata) #1

attributes #0 = { nofree nounwind sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable }
attributes #2 = { argmemonly nounwind }
attributes #3 = { nofree nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { nounwind }
attributes #6 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #7 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #8 = { norecurse nounwind readonly sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #9 = { nofree norecurse nounwind sspstrong uwtable writeonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #10 = { noreturn nounwind sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #11 = { nofree "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #12 = { cold }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!16, !17, !18, !19}
!llvm.ident = !{!20}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 9.0.0 (tags/RELEASE_900/final)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, retainedTypes: !3, nameTableKind: None)
!1 = !DIFile(filename: "test.c", directory: "/home/Test/Documents/nlp-lex/src/playground")
!2 = !{}
!3 = !{!4, !5, !6, !7, !8, !11, !14, !15}
!4 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!5 = !DIBasicType(name: "double", size: 64, encoding: DW_ATE_float)
!6 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!7 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !4, size: 64)
!8 = !DIDerivedType(tag: DW_TAG_typedef, name: "size_t", file: !9, line: 46, baseType: !10)
!9 = !DIFile(filename: "/usr/lib/clang/9.0.0/include/stddef.h", directory: "")
!10 = !DIBasicType(name: "long unsigned int", size: 64, encoding: DW_ATE_unsigned)
!11 = !DIDerivedType(tag: DW_TAG_typedef, name: "__intptr_t", file: !12, line: 206, baseType: !13)
!12 = !DIFile(filename: "/usr/include/bits/types.h", directory: "")
!13 = !DIBasicType(name: "long int", size: 64, encoding: DW_ATE_signed)
!14 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64)
!15 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !5, size: 64)
!16 = !{i32 2, !"Dwarf Version", i32 4}
!17 = !{i32 2, !"Debug Info Version", i32 3}
!18 = !{i32 1, !"wchar_size", i32 4}
!19 = !{i32 7, !"PIC Level", i32 1}
!20 = !{!"clang version 9.0.0 (tags/RELEASE_900/final)"}
!21 = distinct !DISubprogram(name: "putchard", scope: !1, file: !1, line: 166, type: !22, scopeLine: 166, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !24)
!22 = !DISubroutineType(types: !23)
!23 = !{!5, !5}
!24 = !{!25, !26, !27}
!25 = !DILocalVariable(name: "d", arg: 1, scope: !21, file: !1, line: 166, type: !5)
!26 = !DILocalVariable(name: "c", scope: !21, file: !1, line: 167, type: !4)
!27 = !DILocalVariable(name: "x", scope: !21, file: !1, line: 168, type: !6)
!28 = !DILocation(line: 0, scope: !21)
!29 = !DILocation(line: 167, column: 12, scope: !21)
!30 = !DILocation(line: 168, column: 19, scope: !21)
!31 = !{!32, !32, i64 0}
!32 = !{!"any pointer", !33, i64 0}
!33 = !{!"omnipotent char", !34, i64 0}
!34 = !{!"Simple C/C++ TBAA"}
!35 = !DILocation(line: 168, column: 33, scope: !21)
!36 = !DILocation(line: 168, column: 11, scope: !21)
!37 = !DILocation(line: 169, column: 10, scope: !21)
!38 = !DILocation(line: 169, column: 3, scope: !21)
!39 = distinct !DISubprogram(name: "eputchard", scope: !1, file: !1, line: 172, type: !22, scopeLine: 172, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !40)
!40 = !{!41, !42, !43}
!41 = !DILocalVariable(name: "d", arg: 1, scope: !39, file: !1, line: 172, type: !5)
!42 = !DILocalVariable(name: "c", scope: !39, file: !1, line: 173, type: !4)
!43 = !DILocalVariable(name: "x", scope: !39, file: !1, line: 174, type: !6)
!44 = !DILocation(line: 0, scope: !39)
!45 = !DILocation(line: 173, column: 12, scope: !39)
!46 = !DILocation(line: 174, column: 19, scope: !39)
!47 = !DILocation(line: 174, column: 33, scope: !39)
!48 = !DILocation(line: 174, column: 11, scope: !39)
!49 = !DILocation(line: 175, column: 10, scope: !39)
!50 = !DILocation(line: 175, column: 3, scope: !39)
!51 = distinct !DISubprogram(name: "printd", scope: !1, file: !1, line: 178, type: !22, scopeLine: 178, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !52)
!52 = !{!53}
!53 = !DILocalVariable(name: "d", arg: 1, scope: !51, file: !1, line: 178, type: !5)
!54 = !DILocation(line: 0, scope: !51)
!55 = !DILocation(line: 179, column: 19, scope: !51)
!56 = !DILocation(line: 179, column: 10, scope: !51)
!57 = !DILocation(line: 179, column: 3, scope: !51)
!58 = distinct !DISubprogram(name: "eprintd", scope: !1, file: !1, line: 182, type: !22, scopeLine: 182, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !59)
!59 = !{!60}
!60 = !DILocalVariable(name: "d", arg: 1, scope: !58, file: !1, line: 182, type: !5)
!61 = !DILocation(line: 0, scope: !58)
!62 = !DILocation(line: 183, column: 27, scope: !58)
!63 = !DILocation(line: 183, column: 19, scope: !58)
!64 = !DILocation(line: 183, column: 10, scope: !58)
!65 = !DILocation(line: 183, column: 3, scope: !58)
!66 = distinct !DISubprogram(name: "dprintdf", scope: !1, file: !1, line: 186, type: !67, scopeLine: 186, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !69)
!67 = !DISubroutineType(types: !68)
!68 = !{!5, !5, !5, null}
!69 = !{!70, !71, !72, !88, !89}
!70 = !DILocalVariable(name: "fd", arg: 1, scope: !66, file: !1, line: 186, type: !5)
!71 = !DILocalVariable(name: "ptr", arg: 2, scope: !66, file: !1, line: 186, type: !5)
!72 = !DILocalVariable(name: "ap", scope: !66, file: !1, line: 187, type: !73)
!73 = !DIDerivedType(tag: DW_TAG_typedef, name: "va_list", file: !74, line: 52, baseType: !75)
!74 = !DIFile(filename: "/usr/include/stdio.h", directory: "")
!75 = !DIDerivedType(tag: DW_TAG_typedef, name: "__gnuc_va_list", file: !76, line: 32, baseType: !77)
!76 = !DIFile(filename: "/usr/lib/clang/9.0.0/include/stdarg.h", directory: "")
!77 = !DIDerivedType(tag: DW_TAG_typedef, name: "__builtin_va_list", file: !1, line: 187, baseType: !78)
!78 = !DICompositeType(tag: DW_TAG_array_type, baseType: !79, size: 192, elements: !86)
!79 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "__va_list_tag", file: !1, line: 187, size: 192, elements: !80)
!80 = !{!81, !83, !84, !85}
!81 = !DIDerivedType(tag: DW_TAG_member, name: "gp_offset", scope: !79, file: !1, line: 187, baseType: !82, size: 32)
!82 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!83 = !DIDerivedType(tag: DW_TAG_member, name: "fp_offset", scope: !79, file: !1, line: 187, baseType: !82, size: 32, offset: 32)
!84 = !DIDerivedType(tag: DW_TAG_member, name: "overflow_arg_area", scope: !79, file: !1, line: 187, baseType: !14, size: 64, offset: 64)
!85 = !DIDerivedType(tag: DW_TAG_member, name: "reg_save_area", scope: !79, file: !1, line: 187, baseType: !14, size: 64, offset: 128)
!86 = !{!87}
!87 = !DISubrange(count: 1)
!88 = !DILocalVariable(name: "ip", scope: !66, file: !1, line: 190, type: !11)
!89 = !DILocalVariable(name: "res", scope: !66, file: !1, line: 191, type: !5)
!90 = !DILocation(line: 0, scope: !66)
!91 = !DILocation(line: 187, column: 3, scope: !66)
!92 = !DILocation(line: 187, column: 11, scope: !66)
!93 = !DILocation(line: 188, column: 3, scope: !66)
!94 = !DILocation(line: 190, column: 19, scope: !66)
!95 = !DILocation(line: 191, column: 34, scope: !66)
!96 = !DILocation(line: 191, column: 44, scope: !66)
!97 = !DILocation(line: 191, column: 25, scope: !66)
!98 = !DILocation(line: 191, column: 16, scope: !66)
!99 = !DILocation(line: 192, column: 3, scope: !66)
!100 = !DILocation(line: 194, column: 1, scope: !66)
!101 = !DILocation(line: 193, column: 3, scope: !66)
!102 = distinct !DISubprogram(name: "mallocd", scope: !1, file: !1, line: 196, type: !22, scopeLine: 196, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !103)
!103 = !{!104, !105, !106, !107}
!104 = !DILocalVariable(name: "s", arg: 1, scope: !102, file: !1, line: 196, type: !5)
!105 = !DILocalVariable(name: "sze", scope: !102, file: !1, line: 197, type: !8)
!106 = !DILocalVariable(name: "mem", scope: !102, file: !1, line: 198, type: !14)
!107 = !DILocalVariable(name: "ip", scope: !102, file: !1, line: 199, type: !11)
!108 = !DILocation(line: 0, scope: !102)
!109 = !DILocation(line: 197, column: 16, scope: !102)
!110 = !DILocation(line: 198, column: 15, scope: !102)
!111 = !DILocation(line: 199, column: 19, scope: !102)
!112 = !DILocation(line: 200, column: 10, scope: !102)
!113 = !DILocation(line: 200, column: 3, scope: !102)
!114 = distinct !DISubprogram(name: "memsetd", scope: !1, file: !1, line: 203, type: !115, scopeLine: 203, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !117)
!115 = !DISubroutineType(types: !116)
!116 = !{!5, !5, !5, !5}
!117 = !{!118, !119, !120, !121}
!118 = !DILocalVariable(name: "p", arg: 1, scope: !114, file: !1, line: 203, type: !5)
!119 = !DILocalVariable(name: "s", arg: 2, scope: !114, file: !1, line: 203, type: !5)
!120 = !DILocalVariable(name: "n", arg: 3, scope: !114, file: !1, line: 203, type: !5)
!121 = !DILocalVariable(name: "ip", scope: !114, file: !1, line: 204, type: !11)
!122 = !DILocation(line: 0, scope: !114)
!123 = !DILocation(line: 204, column: 19, scope: !114)
!124 = !DILocation(line: 205, column: 28, scope: !114)
!125 = !DILocation(line: 205, column: 40, scope: !114)
!126 = !DILocation(line: 205, column: 21, scope: !114)
!127 = !DILocation(line: 205, column: 49, scope: !114)
!128 = !DILocation(line: 206, column: 10, scope: !114)
!129 = !DILocation(line: 206, column: 3, scope: !114)
!130 = distinct !DISubprogram(name: "freed", scope: !1, file: !1, line: 209, type: !22, scopeLine: 209, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !131)
!131 = !{!132, !133}
!132 = !DILocalVariable(name: "ptr", arg: 1, scope: !130, file: !1, line: 209, type: !5)
!133 = !DILocalVariable(name: "p", scope: !130, file: !1, line: 210, type: !11)
!134 = !DILocation(line: 0, scope: !130)
!135 = !DILocation(line: 210, column: 18, scope: !130)
!136 = !DILocation(line: 211, column: 8, scope: !130)
!137 = !DILocation(line: 211, column: 3, scope: !130)
!138 = !DILocation(line: 212, column: 3, scope: !130)
!139 = distinct !DISubprogram(name: "cderef", scope: !1, file: !1, line: 215, type: !22, scopeLine: 215, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !140)
!140 = !{!141, !142}
!141 = !DILocalVariable(name: "ptr", arg: 1, scope: !139, file: !1, line: 215, type: !5)
!142 = !DILocalVariable(name: "p", scope: !139, file: !1, line: 216, type: !11)
!143 = !DILocation(line: 0, scope: !139)
!144 = !DILocation(line: 216, column: 18, scope: !139)
!145 = !DILocation(line: 217, column: 13, scope: !139)
!146 = !DILocation(line: 217, column: 10, scope: !139)
!147 = !{!33, !33, i64 0}
!148 = !DILocation(line: 217, column: 3, scope: !139)
!149 = distinct !DISubprogram(name: "cderefset", scope: !1, file: !1, line: 220, type: !150, scopeLine: 220, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !152)
!150 = !DISubroutineType(types: !151)
!151 = !{!5, !5, !5}
!152 = !{!153, !154, !155}
!153 = !DILocalVariable(name: "ptr", arg: 1, scope: !149, file: !1, line: 220, type: !5)
!154 = !DILocalVariable(name: "value", arg: 2, scope: !149, file: !1, line: 220, type: !5)
!155 = !DILocalVariable(name: "p", scope: !149, file: !1, line: 221, type: !11)
!156 = !DILocation(line: 0, scope: !149)
!157 = !DILocation(line: 221, column: 18, scope: !149)
!158 = !DILocation(line: 222, column: 35, scope: !149)
!159 = !DILocation(line: 222, column: 22, scope: !149)
!160 = !DILocation(line: 222, column: 33, scope: !149)
!161 = !DILocation(line: 222, column: 10, scope: !149)
!162 = !DILocation(line: 222, column: 3, scope: !149)
!163 = distinct !DISubprogram(name: "dderef", scope: !1, file: !1, line: 225, type: !22, scopeLine: 225, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !164)
!164 = !{!165, !166}
!165 = !DILocalVariable(name: "ptr", arg: 1, scope: !163, file: !1, line: 225, type: !5)
!166 = !DILocalVariable(name: "p", scope: !163, file: !1, line: 226, type: !11)
!167 = !DILocation(line: 0, scope: !163)
!168 = !DILocation(line: 226, column: 18, scope: !163)
!169 = !DILocation(line: 227, column: 13, scope: !163)
!170 = !DILocation(line: 227, column: 10, scope: !163)
!171 = !{!172, !172, i64 0}
!172 = !{!"double", !33, i64 0}
!173 = !DILocation(line: 227, column: 3, scope: !163)
!174 = distinct !DISubprogram(name: "dderefset", scope: !1, file: !1, line: 230, type: !150, scopeLine: 230, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !175)
!175 = !{!176, !177, !178}
!176 = !DILocalVariable(name: "ptr", arg: 1, scope: !174, file: !1, line: 230, type: !5)
!177 = !DILocalVariable(name: "value", arg: 2, scope: !174, file: !1, line: 230, type: !5)
!178 = !DILocalVariable(name: "p", scope: !174, file: !1, line: 231, type: !11)
!179 = !DILocation(line: 0, scope: !174)
!180 = !DILocation(line: 231, column: 18, scope: !174)
!181 = !DILocation(line: 232, column: 13, scope: !174)
!182 = !DILocation(line: 232, column: 26, scope: !174)
!183 = !DILocation(line: 232, column: 3, scope: !174)
!184 = distinct !DISubprogram(name: "trap", scope: !1, file: !1, line: 235, type: !185, scopeLine: 235, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !2)
!185 = !DISubroutineType(types: !186)
!186 = !{!5}
!187 = !DILocation(line: 236, column: 3, scope: !184)
!188 = !{i32 5505}
!189 = !DILocation(line: 237, column: 3, scope: !184)
!190 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 241, type: !191, scopeLine: 241, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !193)
!191 = !DISubroutineType(types: !192)
!192 = !{!6}
!193 = !{!194, !207, !208, !209, !211, !212}
!194 = !DILocalVariable(name: "res", scope: !190, file: !1, line: 243, type: !195)
!195 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "sresult", file: !1, line: 8, size: 384, elements: !196)
!196 = !{!197, !200, !201, !202, !203, !205, !206}
!197 = !DIDerivedType(tag: DW_TAG_member, name: "start", scope: !195, file: !1, line: 9, baseType: !198, size: 64)
!198 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !199, size: 64)
!199 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !4)
!200 = !DIDerivedType(tag: DW_TAG_member, name: "length", scope: !195, file: !1, line: 10, baseType: !6, size: 32, offset: 64)
!201 = !DIDerivedType(tag: DW_TAG_member, name: "tag", scope: !195, file: !1, line: 11, baseType: !198, size: 64, offset: 128)
!202 = !DIDerivedType(tag: DW_TAG_member, name: "errc", scope: !195, file: !1, line: 12, baseType: !4, size: 8, offset: 192)
!203 = !DIDerivedType(tag: DW_TAG_member, name: "metadata", scope: !195, file: !1, line: 13, baseType: !204, size: 8, offset: 200)
!204 = !DIBasicType(name: "unsigned char", size: 8, encoding: DW_ATE_unsigned_char)
!205 = !DIDerivedType(tag: DW_TAG_member, name: "pos", scope: !195, file: !1, line: 14, baseType: !198, size: 64, offset: 256)
!206 = !DIDerivedType(tag: DW_TAG_member, name: "allocd", scope: !195, file: !1, line: 15, baseType: !6, size: 32, offset: 320)
!207 = !DILocalVariable(name: "size", scope: !190, file: !1, line: 244, type: !8)
!208 = !DILocalVariable(name: "s", scope: !190, file: !1, line: 245, type: !7)
!209 = !DILocalVariable(name: "last", scope: !210, file: !1, line: 248, type: !6)
!210 = distinct !DILexicalBlock(scope: !190, file: !1, line: 247, column: 13)
!211 = !DILocalVariable(name: "els", scope: !210, file: !1, line: 249, type: !8)
!212 = !DILocalVariable(name: "dist", scope: !213, file: !1, line: 272, type: !6)
!213 = distinct !DILexicalBlock(scope: !210, file: !1, line: 265, column: 48)
!214 = !DILocation(line: 243, column: 3, scope: !190)
!215 = !DILocation(line: 243, column: 18, scope: !190)
!216 = !DILocation(line: 244, column: 3, scope: !190)
!217 = !DILocation(line: 0, scope: !190)
!218 = !DILocation(line: 244, column: 10, scope: !190)
!219 = !{!220, !220, i64 0}
!220 = !{!"long", !33, i64 0}
!221 = !DILocation(line: 245, column: 3, scope: !190)
!222 = !DILocation(line: 245, column: 13, scope: !190)
!223 = !DILocation(line: 245, column: 9, scope: !190)
!224 = !DILocation(line: 246, column: 3, scope: !190)
!225 = !DILocation(line: 0, scope: !213)
!226 = !DILocation(line: 247, column: 3, scope: !190)
!227 = !DILocation(line: 0, scope: !210)
!228 = !DILocation(line: 250, column: 9, scope: !229)
!229 = distinct !DILexicalBlock(scope: !210, file: !1, line: 250, column: 9)
!230 = !DILocation(line: 250, column: 9, scope: !210)
!231 = !DILocation(line: 251, column: 9, scope: !232)
!232 = distinct !DILexicalBlock(scope: !229, file: !1, line: 250, column: 31)
!233 = !DILocation(line: 252, column: 34, scope: !232)
!234 = !DILocation(line: 252, column: 15, scope: !232)
!235 = !DILocation(line: 253, column: 5, scope: !232)
!236 = !DILocation(line: 254, column: 34, scope: !237)
!237 = distinct !DILexicalBlock(scope: !229, file: !1, line: 253, column: 12)
!238 = !DILocation(line: 254, column: 15, scope: !237)
!239 = !DILocation(line: 259, column: 17, scope: !240)
!240 = distinct !DILexicalBlock(scope: !237, file: !1, line: 259, column: 13)
!241 = !DILocation(line: 259, column: 13, scope: !237)
!242 = distinct !{!242, !226, !243}
!243 = !DILocation(line: 277, column: 3, scope: !190)
!244 = !DILocation(line: 0, scope: !229)
!245 = !DILocation(line: 261, column: 5, scope: !210)
!246 = !DILocation(line: 261, column: 11, scope: !210)
!247 = !DILocation(line: 261, column: 16, scope: !210)
!248 = !DILocation(line: 262, column: 35, scope: !210)
!249 = !DILocation(line: 262, column: 5, scope: !210)
!250 = !DILocation(line: 264, column: 17, scope: !210)
!251 = !DILocation(line: 264, column: 5, scope: !210)
!252 = !DILocation(line: 265, column: 5, scope: !210)
!253 = !DILocation(line: 248, column: 9, scope: !210)
!254 = !DILocation(line: 267, column: 7, scope: !213)
!255 = !DILocation(line: 270, column: 19, scope: !213)
!256 = !{!257, !33, i64 24}
!257 = !{!"sresult", !32, i64 0, !258, i64 8, !32, i64 16, !33, i64 24, !33, i64 25, !32, i64 32, !258, i64 40}
!258 = !{!"int", !33, i64 0}
!259 = !DILocation(line: 270, column: 15, scope: !213)
!260 = !DILocation(line: 270, column: 43, scope: !213)
!261 = !{!257, !258, i64 8}
!262 = !DILocation(line: 270, column: 55, scope: !213)
!263 = !{!257, !32, i64 0}
!264 = !DILocation(line: 270, column: 66, scope: !213)
!265 = !{!257, !32, i64 32}
!266 = !DILocation(line: 271, column: 30, scope: !213)
!267 = !{!257, !32, i64 16}
!268 = !DILocation(line: 271, column: 40, scope: !213)
!269 = !{!257, !33, i64 25}
!270 = !DILocation(line: 271, column: 49, scope: !213)
!271 = !DILocation(line: 271, column: 36, scope: !213)
!272 = !DILocation(line: 269, column: 7, scope: !213)
!273 = !DILocation(line: 272, column: 18, scope: !213)
!274 = !DILocation(line: 273, column: 15, scope: !275)
!275 = distinct !DILexicalBlock(scope: !213, file: !1, line: 273, column: 11)
!276 = !DILocation(line: 273, column: 11, scope: !275)
!277 = !DILocation(line: 273, column: 27, scope: !275)
!278 = !DILocation(line: 273, column: 34, scope: !275)
!279 = !DILocation(line: 273, column: 20, scope: !275)
!280 = !DILocation(line: 273, column: 47, scope: !275)
!281 = distinct !{!281, !252, !282}
!282 = !DILocation(line: 276, column: 5, scope: !210)
