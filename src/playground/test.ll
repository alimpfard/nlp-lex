; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i64, i16, i8, [1 x i8], i8*, i64, %struct._IO_codecvt*, %struct._IO_wide_data*, %struct._IO_FILE*, i8*, i64, i32, [20 x i8] }
%struct._IO_marker = type opaque
%struct._IO_codecvt = type opaque
%struct._IO_wide_data = type opaque
%struct.sresult = type { i8*, i32, i8*, i8, i8, i8*, i32 }

@.str = private unnamed_addr constant [4 x i8] c"%lf\00", align 1
@stderr = external local_unnamed_addr global %struct._IO_FILE*, align 8
@.str.1 = private unnamed_addr constant [20 x i8] c"res at %p, s at %p\0A\00", align 1
@.str.2 = private unnamed_addr constant [3 x i8] c"> \00", align 1
@stdin = external local_unnamed_addr global %struct._IO_FILE*, align 8
@.str.3 = private unnamed_addr constant [19 x i8] c"processing - '%s'\0A\00", align 1
@.str.4 = private unnamed_addr constant [46 x i8] c"%smatch {'%.*s' - %s - %d %s} is%sa stopword\0A\00", align 1
@.str.5 = private unnamed_addr constant [4 x i8] c"no \00", align 1
@.str.6 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@.str.7 = private unnamed_addr constant [2 x i8] c" \00", align 1
@.str.8 = private unnamed_addr constant [6 x i8] c" not \00", align 1
@stdout = external local_unnamed_addr global %struct._IO_FILE*, align 8

; Function Attrs: nofree nounwind sspstrong uwtable
define double @putchard(double) local_unnamed_addr #0 !dbg !20 {
  call void @llvm.dbg.value(metadata double %0, metadata !24, metadata !DIExpression()), !dbg !27
  %2 = fptosi double %0 to i8, !dbg !28
  call void @llvm.dbg.value(metadata i8 %2, metadata !25, metadata !DIExpression()), !dbg !27
  %3 = sext i8 %2 to i32, !dbg !29
  call void @llvm.dbg.value(metadata i32 %3, metadata !30, metadata !DIExpression()) #9, !dbg !36
  %4 = load %struct._IO_FILE*, %struct._IO_FILE** @stdout, align 8, !dbg !38, !tbaa !39
  %5 = tail call i32 @putc(i32 %3, %struct._IO_FILE* %4) #9, !dbg !43
  call void @llvm.dbg.value(metadata i32 %5, metadata !26, metadata !DIExpression()), !dbg !27
  %6 = sitofp i32 %5 to double, !dbg !44
  ret double %6, !dbg !45
}

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: nofree nounwind sspstrong uwtable
define double @printd(double) local_unnamed_addr #0 !dbg !46 {
  call void @llvm.dbg.value(metadata double %0, metadata !48, metadata !DIExpression()), !dbg !49
  %2 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), double %0), !dbg !50
  %3 = sitofp i32 %2 to double, !dbg !51
  ret double %3, !dbg !52
}

; Function Attrs: nofree nounwind
declare i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #2

; Function Attrs: nofree nounwind sspstrong uwtable
define double @eprintd(double) local_unnamed_addr #0 !dbg !53 {
  call void @llvm.dbg.value(metadata double %0, metadata !55, metadata !DIExpression()), !dbg !56
  %2 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 8, !dbg !57, !tbaa !39
  %3 = tail call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %2, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), double %0) #10, !dbg !58
  %4 = sitofp i32 %3 to double, !dbg !59
  ret double %4, !dbg !60
}

; Function Attrs: nofree nounwind
declare i32 @fprintf(%struct._IO_FILE* nocapture, i8* nocapture readonly, ...) local_unnamed_addr #2

; Function Attrs: nofree nounwind sspstrong uwtable
define double @mallocd(double) local_unnamed_addr #0 !dbg !61 {
  call void @llvm.dbg.value(metadata double %0, metadata !63, metadata !DIExpression()), !dbg !67
  %2 = fptoui double %0 to i64, !dbg !68
  call void @llvm.dbg.value(metadata i64 %2, metadata !64, metadata !DIExpression()), !dbg !67
  %3 = tail call i8* @malloc(i64 %2), !dbg !69
  call void @llvm.dbg.value(metadata i8* %3, metadata !65, metadata !DIExpression()), !dbg !67
  %4 = ptrtoint i8* %3 to i64, !dbg !70
  call void @llvm.dbg.value(metadata i64 %4, metadata !66, metadata !DIExpression()), !dbg !67
  %5 = sitofp i64 %4 to double, !dbg !71
  ret double %5, !dbg !72
}

; Function Attrs: nofree nounwind
declare noalias i8* @malloc(i64) local_unnamed_addr #2

; Function Attrs: nounwind sspstrong uwtable
define double @memsetd(double, double, double) local_unnamed_addr #3 !dbg !73 {
  call void @llvm.dbg.value(metadata double %0, metadata !77, metadata !DIExpression()), !dbg !81
  call void @llvm.dbg.value(metadata double %1, metadata !78, metadata !DIExpression()), !dbg !81
  call void @llvm.dbg.value(metadata double %2, metadata !79, metadata !DIExpression()), !dbg !81
  %4 = fptosi double %0 to i64, !dbg !82
  call void @llvm.dbg.value(metadata i64 %4, metadata !80, metadata !DIExpression()), !dbg !81
  %5 = inttoptr i64 %4 to i8*, !dbg !83
  %6 = fptosi double %1 to i32, !dbg !84
  %7 = trunc i32 %6 to i8, !dbg !85
  %8 = fptoui double %2 to i64, !dbg !86
  tail call void @llvm.memset.p0i8.i64(i8* align 1 %5, i8 %7, i64 %8, i1 false), !dbg !85
  call void @llvm.dbg.value(metadata i64 %4, metadata !80, metadata !DIExpression()), !dbg !81
  %9 = sitofp i64 %4 to double, !dbg !87
  ret double %9, !dbg !88
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #1

; Function Attrs: nounwind sspstrong uwtable
define double @freed(double) local_unnamed_addr #3 !dbg !89 {
  call void @llvm.dbg.value(metadata double %0, metadata !91, metadata !DIExpression()), !dbg !93
  %2 = fptosi double %0 to i64, !dbg !94
  call void @llvm.dbg.value(metadata i64 %2, metadata !92, metadata !DIExpression()), !dbg !93
  %3 = inttoptr i64 %2 to i8*, !dbg !95
  %4 = tail call i32 (i8*, ...) bitcast (i32 (...)* @free to i32 (i8*, ...)*)(i8* %3) #9, !dbg !96
  %5 = sitofp i32 %4 to double, !dbg !97
  ret double %5, !dbg !98
}

declare i32 @free(...) local_unnamed_addr #4

; Function Attrs: norecurse nounwind readonly sspstrong uwtable
define double @cderef(double) local_unnamed_addr #5 !dbg !99 {
  call void @llvm.dbg.value(metadata double %0, metadata !101, metadata !DIExpression()), !dbg !103
  %2 = fptosi double %0 to i64, !dbg !104
  call void @llvm.dbg.value(metadata i64 %2, metadata !102, metadata !DIExpression()), !dbg !103
  %3 = inttoptr i64 %2 to i8*, !dbg !105
  %4 = load i8, i8* %3, align 1, !dbg !106, !tbaa !107
  %5 = sitofp i8 %4 to double, !dbg !106
  ret double %5, !dbg !108
}

; Function Attrs: nofree norecurse nounwind sspstrong uwtable writeonly
define double @cderefset(double, double) local_unnamed_addr #6 !dbg !109 {
  call void @llvm.dbg.value(metadata double %0, metadata !113, metadata !DIExpression()), !dbg !116
  call void @llvm.dbg.value(metadata double %1, metadata !114, metadata !DIExpression()), !dbg !116
  %3 = fptosi double %0 to i64, !dbg !117
  call void @llvm.dbg.value(metadata i64 %3, metadata !115, metadata !DIExpression()), !dbg !116
  %4 = fptosi double %1 to i8, !dbg !118
  %5 = inttoptr i64 %3 to i8*, !dbg !119
  store i8 %4, i8* %5, align 1, !dbg !120, !tbaa !107
  %6 = sitofp i8 %4 to double, !dbg !121
  ret double %6, !dbg !122
}

; Function Attrs: noreturn nounwind sspstrong uwtable
define i32 @main() local_unnamed_addr #7 !dbg !123 {
  %1 = alloca %struct.sresult, align 8
  %2 = alloca i64, align 8
  %3 = alloca i8*, align 8
  %4 = bitcast %struct.sresult* %1 to i8*, !dbg !147
  call void @llvm.lifetime.start.p0i8(i64 48, i8* nonnull %4) #9, !dbg !147
  call void @llvm.memset.p0i8.i64(i8* nonnull align 8 %4, i8 0, i64 48, i1 false), !dbg !148
  %5 = bitcast i64* %2 to i8*, !dbg !149
  call void @llvm.lifetime.start.p0i8(i64 8, i8* nonnull %5) #9, !dbg !149
  call void @llvm.dbg.value(metadata i64 1024, metadata !140, metadata !DIExpression()), !dbg !150
  store i64 1024, i64* %2, align 8, !dbg !151, !tbaa !152
  %6 = bitcast i8** %3 to i8*, !dbg !154
  call void @llvm.lifetime.start.p0i8(i64 8, i8* nonnull %6) #9, !dbg !154
  call void @llvm.dbg.value(metadata i64 1024, metadata !140, metadata !DIExpression()), !dbg !150
  %7 = tail call i8* @malloc(i64 1024), !dbg !155
  call void @llvm.dbg.value(metadata i8* %7, metadata !141, metadata !DIExpression()), !dbg !150
  store i8* %7, i8** %3, align 8, !dbg !156, !tbaa !39
  call void @llvm.dbg.value(metadata %struct.sresult* %1, metadata !127, metadata !DIExpression(DW_OP_deref)), !dbg !150
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.1, i64 0, i64 0), %struct.sresult* nonnull %1, i8* %7), !dbg !157
  %9 = getelementptr inbounds %struct.sresult, %struct.sresult* %1, i64 0, i32 3, !dbg !158
  %10 = getelementptr inbounds %struct.sresult, %struct.sresult* %1, i64 0, i32 1, !dbg !158
  %11 = getelementptr inbounds %struct.sresult, %struct.sresult* %1, i64 0, i32 0, !dbg !158
  %12 = getelementptr inbounds %struct.sresult, %struct.sresult* %1, i64 0, i32 5, !dbg !158
  %13 = getelementptr inbounds %struct.sresult, %struct.sresult* %1, i64 0, i32 2, !dbg !158
  %14 = getelementptr inbounds %struct.sresult, %struct.sresult* %1, i64 0, i32 4, !dbg !158
  br label %15, !dbg !159

15:                                               ; preds = %25, %0
  call void @llvm.dbg.value(metadata i32 -1, metadata !142, metadata !DIExpression()), !dbg !160
  %16 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.2, i64 0, i64 0)), !dbg !161
  %17 = load %struct._IO_FILE*, %struct._IO_FILE** @stdin, align 8, !dbg !162, !tbaa !39
  call void @llvm.dbg.value(metadata i64* %2, metadata !140, metadata !DIExpression(DW_OP_deref)), !dbg !150
  call void @llvm.dbg.value(metadata i8** %3, metadata !141, metadata !DIExpression(DW_OP_deref)), !dbg !150
  %18 = call i64 @getline(i8** nonnull %3, i64* nonnull %2, %struct._IO_FILE* %17) #9, !dbg !163
  call void @llvm.dbg.value(metadata i64 %18, metadata !144, metadata !DIExpression()), !dbg !160
  %19 = load i8*, i8** %3, align 8, !dbg !164, !tbaa !39
  call void @llvm.dbg.value(metadata i8* %19, metadata !141, metadata !DIExpression()), !dbg !150
  %20 = add i64 %18, -1, !dbg !165
  %21 = getelementptr inbounds i8, i8* %19, i64 %20, !dbg !164
  store i8 0, i8* %21, align 1, !dbg !166, !tbaa !107
  %22 = load i8*, i8** %3, align 8, !dbg !167, !tbaa !39
  call void @llvm.dbg.value(metadata i8* %22, metadata !141, metadata !DIExpression()), !dbg !150
  %23 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.3, i64 0, i64 0), i8* %22), !dbg !168
  %24 = load i8*, i8** %3, align 8, !dbg !169, !tbaa !39
  call void @llvm.dbg.value(metadata i8* %24, metadata !141, metadata !DIExpression()), !dbg !150
  call void @__nlex_feed(i8* %24) #9, !dbg !170
  br label %25, !dbg !171

25:                                               ; preds = %25, %15
  %26 = phi i32 [ -1, %15 ], [ %39, %25 ], !dbg !172
  call void @llvm.dbg.value(metadata i32 %26, metadata !142, metadata !DIExpression()), !dbg !160
  call void @llvm.dbg.value(metadata %struct.sresult* %1, metadata !127, metadata !DIExpression(DW_OP_deref)), !dbg !150
  call void @__nlex_root(%struct.sresult* nonnull %1) #9, !dbg !173
  %27 = load i8, i8* %9, align 8, !dbg !174, !tbaa !175
  %28 = icmp eq i8 %27, 0, !dbg !178
  %29 = select i1 %28, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str.6, i64 0, i64 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.5, i64 0, i64 0), !dbg !178
  %30 = load i32, i32* %10, align 8, !dbg !179, !tbaa !180
  %31 = load i8*, i8** %11, align 8, !dbg !181, !tbaa !182
  %32 = load i8*, i8** %12, align 8, !dbg !183, !tbaa !184
  %33 = load i8*, i8** %13, align 8, !dbg !185, !tbaa !186
  %34 = load i8, i8* %14, align 1, !dbg !187, !tbaa !188
  %35 = and i8 %34, 1, !dbg !189
  %36 = icmp eq i8 %35, 0, !dbg !190
  %37 = select i1 %36, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.8, i64 0, i64 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.7, i64 0, i64 0), !dbg !190
  %38 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([46 x i8], [46 x i8]* @.str.4, i64 0, i64 0), i8* %29, i32 %30, i8* %31, i8* %32, i32 %30, i8* %33, i8* %37), !dbg !191
  %39 = call i32 (...) @__nlex_distance() #9, !dbg !192
  call void @llvm.dbg.value(metadata i32 %39, metadata !145, metadata !DIExpression()), !dbg !158
  %40 = load i8, i8* %9, align 8, !dbg !193, !tbaa !175
  %41 = icmp ne i8 %40, 0, !dbg !195
  %42 = load i32, i32* %10, align 8, !dbg !196
  %43 = icmp eq i32 %42, 0, !dbg !197
  %44 = or i1 %41, %43, !dbg !198
  %45 = icmp eq i32 %39, %26, !dbg !199
  %46 = or i1 %45, %44, !dbg !198
  br i1 %46, label %15, label %25, !llvm.loop !200
}

declare i64 @getline(i8**, i64*, %struct._IO_FILE*) local_unnamed_addr #4

declare void @__nlex_feed(i8*) local_unnamed_addr #4

declare void @__nlex_root(%struct.sresult*) local_unnamed_addr #4

declare i32 @__nlex_distance(...) local_unnamed_addr #4

; Function Attrs: nofree nounwind
declare i32 @putc(i32, %struct._IO_FILE* nocapture) local_unnamed_addr #2

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.value(metadata, metadata, metadata) #8

attributes #0 = { nofree nounwind sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind }
attributes #2 = { nofree nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { norecurse nounwind readonly sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #6 = { nofree norecurse nounwind sspstrong uwtable writeonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #7 = { noreturn nounwind sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #8 = { nounwind readnone speculatable }
attributes #9 = { nounwind }
attributes #10 = { cold }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!15, !16, !17, !18}
!llvm.ident = !{!19}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 9.0.0 (tags/RELEASE_900/final)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, retainedTypes: !3, nameTableKind: None)
!1 = !DIFile(filename: "test.c", directory: "/home/Test/Documents/nlp-lex/src/playground")
!2 = !{}
!3 = !{!4, !5, !6, !9, !12, !13, !14}
!4 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!5 = !DIBasicType(name: "double", size: 64, encoding: DW_ATE_float)
!6 = !DIDerivedType(tag: DW_TAG_typedef, name: "size_t", file: !7, line: 46, baseType: !8)
!7 = !DIFile(filename: "/usr/lib/clang/9.0.0/include/stddef.h", directory: "")
!8 = !DIBasicType(name: "long unsigned int", size: 64, encoding: DW_ATE_unsigned)
!9 = !DIDerivedType(tag: DW_TAG_typedef, name: "__intptr_t", file: !10, line: 206, baseType: !11)
!10 = !DIFile(filename: "/usr/include/bits/types.h", directory: "")
!11 = !DIBasicType(name: "long int", size: 64, encoding: DW_ATE_signed)
!12 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64)
!13 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!14 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !4, size: 64)
!15 = !{i32 2, !"Dwarf Version", i32 4}
!16 = !{i32 2, !"Debug Info Version", i32 3}
!17 = !{i32 1, !"wchar_size", i32 4}
!18 = !{i32 7, !"PIC Level", i32 1}
!19 = !{!"clang version 9.0.0 (tags/RELEASE_900/final)"}
!20 = distinct !DISubprogram(name: "putchard", scope: !1, file: !1, line: 162, type: !21, scopeLine: 162, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !23)
!21 = !DISubroutineType(types: !22)
!22 = !{!5, !5}
!23 = !{!24, !25, !26}
!24 = !DILocalVariable(name: "d", arg: 1, scope: !20, file: !1, line: 162, type: !5)
!25 = !DILocalVariable(name: "c", scope: !20, file: !1, line: 163, type: !4)
!26 = !DILocalVariable(name: "x", scope: !20, file: !1, line: 164, type: !13)
!27 = !DILocation(line: 0, scope: !20)
!28 = !DILocation(line: 163, column: 12, scope: !20)
!29 = !DILocation(line: 164, column: 19, scope: !20)
!30 = !DILocalVariable(name: "__c", arg: 1, scope: !31, file: !32, line: 82, type: !13)
!31 = distinct !DISubprogram(name: "putchar", scope: !32, file: !32, line: 82, type: !33, scopeLine: 83, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !35)
!32 = !DIFile(filename: "/usr/include/bits/stdio.h", directory: "")
!33 = !DISubroutineType(types: !34)
!34 = !{!13, !13}
!35 = !{!30}
!36 = !DILocation(line: 0, scope: !31, inlinedAt: !37)
!37 = distinct !DILocation(line: 164, column: 11, scope: !20)
!38 = !DILocation(line: 84, column: 21, scope: !31, inlinedAt: !37)
!39 = !{!40, !40, i64 0}
!40 = !{!"any pointer", !41, i64 0}
!41 = !{!"omnipotent char", !42, i64 0}
!42 = !{!"Simple C/C++ TBAA"}
!43 = !DILocation(line: 84, column: 10, scope: !31, inlinedAt: !37)
!44 = !DILocation(line: 165, column: 10, scope: !20)
!45 = !DILocation(line: 165, column: 3, scope: !20)
!46 = distinct !DISubprogram(name: "printd", scope: !1, file: !1, line: 168, type: !21, scopeLine: 168, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !47)
!47 = !{!48}
!48 = !DILocalVariable(name: "d", arg: 1, scope: !46, file: !1, line: 168, type: !5)
!49 = !DILocation(line: 0, scope: !46)
!50 = !DILocation(line: 169, column: 19, scope: !46)
!51 = !DILocation(line: 169, column: 10, scope: !46)
!52 = !DILocation(line: 169, column: 3, scope: !46)
!53 = distinct !DISubprogram(name: "eprintd", scope: !1, file: !1, line: 172, type: !21, scopeLine: 172, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !54)
!54 = !{!55}
!55 = !DILocalVariable(name: "d", arg: 1, scope: !53, file: !1, line: 172, type: !5)
!56 = !DILocation(line: 0, scope: !53)
!57 = !DILocation(line: 173, column: 27, scope: !53)
!58 = !DILocation(line: 173, column: 19, scope: !53)
!59 = !DILocation(line: 173, column: 10, scope: !53)
!60 = !DILocation(line: 173, column: 3, scope: !53)
!61 = distinct !DISubprogram(name: "mallocd", scope: !1, file: !1, line: 176, type: !21, scopeLine: 176, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !62)
!62 = !{!63, !64, !65, !66}
!63 = !DILocalVariable(name: "s", arg: 1, scope: !61, file: !1, line: 176, type: !5)
!64 = !DILocalVariable(name: "sze", scope: !61, file: !1, line: 177, type: !6)
!65 = !DILocalVariable(name: "mem", scope: !61, file: !1, line: 178, type: !12)
!66 = !DILocalVariable(name: "ip", scope: !61, file: !1, line: 179, type: !9)
!67 = !DILocation(line: 0, scope: !61)
!68 = !DILocation(line: 177, column: 16, scope: !61)
!69 = !DILocation(line: 178, column: 15, scope: !61)
!70 = !DILocation(line: 179, column: 19, scope: !61)
!71 = !DILocation(line: 180, column: 10, scope: !61)
!72 = !DILocation(line: 180, column: 3, scope: !61)
!73 = distinct !DISubprogram(name: "memsetd", scope: !1, file: !1, line: 183, type: !74, scopeLine: 183, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !76)
!74 = !DISubroutineType(types: !75)
!75 = !{!5, !5, !5, !5}
!76 = !{!77, !78, !79, !80}
!77 = !DILocalVariable(name: "p", arg: 1, scope: !73, file: !1, line: 183, type: !5)
!78 = !DILocalVariable(name: "s", arg: 2, scope: !73, file: !1, line: 183, type: !5)
!79 = !DILocalVariable(name: "n", arg: 3, scope: !73, file: !1, line: 183, type: !5)
!80 = !DILocalVariable(name: "ip", scope: !73, file: !1, line: 184, type: !9)
!81 = !DILocation(line: 0, scope: !73)
!82 = !DILocation(line: 184, column: 19, scope: !73)
!83 = !DILocation(line: 185, column: 28, scope: !73)
!84 = !DILocation(line: 185, column: 40, scope: !73)
!85 = !DILocation(line: 185, column: 21, scope: !73)
!86 = !DILocation(line: 185, column: 49, scope: !73)
!87 = !DILocation(line: 186, column: 10, scope: !73)
!88 = !DILocation(line: 186, column: 3, scope: !73)
!89 = distinct !DISubprogram(name: "freed", scope: !1, file: !1, line: 189, type: !21, scopeLine: 189, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !90)
!90 = !{!91, !92}
!91 = !DILocalVariable(name: "ptr", arg: 1, scope: !89, file: !1, line: 189, type: !5)
!92 = !DILocalVariable(name: "p", scope: !89, file: !1, line: 190, type: !9)
!93 = !DILocation(line: 0, scope: !89)
!94 = !DILocation(line: 190, column: 18, scope: !89)
!95 = !DILocation(line: 191, column: 24, scope: !89)
!96 = !DILocation(line: 191, column: 19, scope: !89)
!97 = !DILocation(line: 191, column: 10, scope: !89)
!98 = !DILocation(line: 191, column: 3, scope: !89)
!99 = distinct !DISubprogram(name: "cderef", scope: !1, file: !1, line: 194, type: !21, scopeLine: 194, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !100)
!100 = !{!101, !102}
!101 = !DILocalVariable(name: "ptr", arg: 1, scope: !99, file: !1, line: 194, type: !5)
!102 = !DILocalVariable(name: "p", scope: !99, file: !1, line: 195, type: !9)
!103 = !DILocation(line: 0, scope: !99)
!104 = !DILocation(line: 195, column: 18, scope: !99)
!105 = !DILocation(line: 196, column: 13, scope: !99)
!106 = !DILocation(line: 196, column: 10, scope: !99)
!107 = !{!41, !41, i64 0}
!108 = !DILocation(line: 196, column: 3, scope: !99)
!109 = distinct !DISubprogram(name: "cderefset", scope: !1, file: !1, line: 199, type: !110, scopeLine: 199, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !112)
!110 = !DISubroutineType(types: !111)
!111 = !{!5, !5, !5}
!112 = !{!113, !114, !115}
!113 = !DILocalVariable(name: "ptr", arg: 1, scope: !109, file: !1, line: 199, type: !5)
!114 = !DILocalVariable(name: "value", arg: 2, scope: !109, file: !1, line: 199, type: !5)
!115 = !DILocalVariable(name: "p", scope: !109, file: !1, line: 200, type: !9)
!116 = !DILocation(line: 0, scope: !109)
!117 = !DILocation(line: 200, column: 18, scope: !109)
!118 = !DILocation(line: 201, column: 35, scope: !109)
!119 = !DILocation(line: 201, column: 22, scope: !109)
!120 = !DILocation(line: 201, column: 33, scope: !109)
!121 = !DILocation(line: 201, column: 10, scope: !109)
!122 = !DILocation(line: 201, column: 3, scope: !109)
!123 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 205, type: !124, scopeLine: 205, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !126)
!124 = !DISubroutineType(types: !125)
!125 = !{!13}
!126 = !{!127, !140, !141, !142, !144, !145}
!127 = !DILocalVariable(name: "res", scope: !123, file: !1, line: 207, type: !128)
!128 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "sresult", file: !1, line: 4, size: 384, elements: !129)
!129 = !{!130, !133, !134, !135, !136, !138, !139}
!130 = !DIDerivedType(tag: DW_TAG_member, name: "start", scope: !128, file: !1, line: 5, baseType: !131, size: 64)
!131 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !132, size: 64)
!132 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !4)
!133 = !DIDerivedType(tag: DW_TAG_member, name: "length", scope: !128, file: !1, line: 6, baseType: !13, size: 32, offset: 64)
!134 = !DIDerivedType(tag: DW_TAG_member, name: "tag", scope: !128, file: !1, line: 7, baseType: !131, size: 64, offset: 128)
!135 = !DIDerivedType(tag: DW_TAG_member, name: "errc", scope: !128, file: !1, line: 8, baseType: !4, size: 8, offset: 192)
!136 = !DIDerivedType(tag: DW_TAG_member, name: "metadata", scope: !128, file: !1, line: 9, baseType: !137, size: 8, offset: 200)
!137 = !DIBasicType(name: "unsigned char", size: 8, encoding: DW_ATE_unsigned_char)
!138 = !DIDerivedType(tag: DW_TAG_member, name: "pos", scope: !128, file: !1, line: 10, baseType: !131, size: 64, offset: 256)
!139 = !DIDerivedType(tag: DW_TAG_member, name: "allocd", scope: !128, file: !1, line: 11, baseType: !13, size: 32, offset: 320)
!140 = !DILocalVariable(name: "size", scope: !123, file: !1, line: 208, type: !6)
!141 = !DILocalVariable(name: "s", scope: !123, file: !1, line: 209, type: !14)
!142 = !DILocalVariable(name: "last", scope: !143, file: !1, line: 212, type: !13)
!143 = distinct !DILexicalBlock(scope: !123, file: !1, line: 211, column: 13)
!144 = !DILocalVariable(name: "els", scope: !143, file: !1, line: 214, type: !6)
!145 = !DILocalVariable(name: "dist", scope: !146, file: !1, line: 226, type: !13)
!146 = distinct !DILexicalBlock(scope: !143, file: !1, line: 219, column: 48)
!147 = !DILocation(line: 207, column: 3, scope: !123)
!148 = !DILocation(line: 207, column: 18, scope: !123)
!149 = !DILocation(line: 208, column: 3, scope: !123)
!150 = !DILocation(line: 0, scope: !123)
!151 = !DILocation(line: 208, column: 10, scope: !123)
!152 = !{!153, !153, i64 0}
!153 = !{!"long", !41, i64 0}
!154 = !DILocation(line: 209, column: 3, scope: !123)
!155 = !DILocation(line: 209, column: 13, scope: !123)
!156 = !DILocation(line: 209, column: 9, scope: !123)
!157 = !DILocation(line: 210, column: 3, scope: !123)
!158 = !DILocation(line: 0, scope: !146)
!159 = !DILocation(line: 211, column: 3, scope: !123)
!160 = !DILocation(line: 0, scope: !143)
!161 = !DILocation(line: 213, column: 5, scope: !143)
!162 = !DILocation(line: 214, column: 37, scope: !143)
!163 = !DILocation(line: 214, column: 18, scope: !143)
!164 = !DILocation(line: 215, column: 5, scope: !143)
!165 = !DILocation(line: 215, column: 11, scope: !143)
!166 = !DILocation(line: 215, column: 16, scope: !143)
!167 = !DILocation(line: 216, column: 35, scope: !143)
!168 = !DILocation(line: 216, column: 5, scope: !143)
!169 = !DILocation(line: 218, column: 17, scope: !143)
!170 = !DILocation(line: 218, column: 5, scope: !143)
!171 = !DILocation(line: 219, column: 5, scope: !143)
!172 = !DILocation(line: 212, column: 9, scope: !143)
!173 = !DILocation(line: 221, column: 7, scope: !146)
!174 = !DILocation(line: 224, column: 19, scope: !146)
!175 = !{!176, !41, i64 24}
!176 = !{!"sresult", !40, i64 0, !177, i64 8, !40, i64 16, !41, i64 24, !41, i64 25, !40, i64 32, !177, i64 40}
!177 = !{!"int", !41, i64 0}
!178 = !DILocation(line: 224, column: 15, scope: !146)
!179 = !DILocation(line: 224, column: 43, scope: !146)
!180 = !{!176, !177, i64 8}
!181 = !DILocation(line: 224, column: 55, scope: !146)
!182 = !{!176, !40, i64 0}
!183 = !DILocation(line: 224, column: 66, scope: !146)
!184 = !{!176, !40, i64 32}
!185 = !DILocation(line: 225, column: 30, scope: !146)
!186 = !{!176, !40, i64 16}
!187 = !DILocation(line: 225, column: 40, scope: !146)
!188 = !{!176, !41, i64 25}
!189 = !DILocation(line: 225, column: 49, scope: !146)
!190 = !DILocation(line: 225, column: 36, scope: !146)
!191 = !DILocation(line: 223, column: 7, scope: !146)
!192 = !DILocation(line: 226, column: 18, scope: !146)
!193 = !DILocation(line: 227, column: 15, scope: !194)
!194 = distinct !DILexicalBlock(scope: !146, file: !1, line: 227, column: 11)
!195 = !DILocation(line: 227, column: 11, scope: !194)
!196 = !DILocation(line: 227, column: 27, scope: !194)
!197 = !DILocation(line: 227, column: 34, scope: !194)
!198 = !DILocation(line: 227, column: 20, scope: !194)
!199 = !DILocation(line: 227, column: 47, scope: !194)
!200 = distinct !{!200, !159, !201}
!201 = !DILocation(line: 231, column: 3, scope: !123)
