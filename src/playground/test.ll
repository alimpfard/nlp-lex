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
define double @mallocd(double) local_unnamed_addr #0 !dbg !53 {
  call void @llvm.dbg.value(metadata double %0, metadata !55, metadata !DIExpression()), !dbg !59
  %2 = fptoui double %0 to i64, !dbg !60
  call void @llvm.dbg.value(metadata i64 %2, metadata !56, metadata !DIExpression()), !dbg !59
  %3 = tail call i8* @malloc(i64 %2), !dbg !61
  call void @llvm.dbg.value(metadata i8* %3, metadata !57, metadata !DIExpression()), !dbg !59
  %4 = ptrtoint i8* %3 to i64, !dbg !62
  call void @llvm.dbg.value(metadata i64 %4, metadata !58, metadata !DIExpression()), !dbg !59
  %5 = sitofp i64 %4 to double, !dbg !63
  ret double %5, !dbg !64
}

; Function Attrs: nofree nounwind
declare noalias i8* @malloc(i64) local_unnamed_addr #2

; Function Attrs: nounwind sspstrong uwtable
define double @memsetd(double, double, double) local_unnamed_addr #3 !dbg !65 {
  call void @llvm.dbg.value(metadata double %0, metadata !69, metadata !DIExpression()), !dbg !73
  call void @llvm.dbg.value(metadata double %1, metadata !70, metadata !DIExpression()), !dbg !73
  call void @llvm.dbg.value(metadata double %2, metadata !71, metadata !DIExpression()), !dbg !73
  %4 = fptosi double %0 to i64, !dbg !74
  call void @llvm.dbg.value(metadata i64 %4, metadata !72, metadata !DIExpression()), !dbg !73
  %5 = inttoptr i64 %4 to i8*, !dbg !75
  %6 = fptosi double %1 to i32, !dbg !76
  %7 = trunc i32 %6 to i8, !dbg !77
  %8 = fptoui double %2 to i64, !dbg !78
  tail call void @llvm.memset.p0i8.i64(i8* align 1 %5, i8 %7, i64 %8, i1 false), !dbg !77
  call void @llvm.dbg.value(metadata i64 %4, metadata !72, metadata !DIExpression()), !dbg !73
  %9 = sitofp i64 %4 to double, !dbg !79
  ret double %9, !dbg !80
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #1

; Function Attrs: nounwind sspstrong uwtable
define double @freed(double) local_unnamed_addr #3 !dbg !81 {
  call void @llvm.dbg.value(metadata double %0, metadata !83, metadata !DIExpression()), !dbg !85
  %2 = fptosi double %0 to i64, !dbg !86
  call void @llvm.dbg.value(metadata i64 %2, metadata !84, metadata !DIExpression()), !dbg !85
  %3 = inttoptr i64 %2 to i8*, !dbg !87
  %4 = tail call i32 (i8*, ...) bitcast (i32 (...)* @free to i32 (i8*, ...)*)(i8* %3) #9, !dbg !88
  %5 = sitofp i32 %4 to double, !dbg !89
  ret double %5, !dbg !90
}

declare i32 @free(...) local_unnamed_addr #4

; Function Attrs: norecurse nounwind readonly sspstrong uwtable
define double @cderef(double) local_unnamed_addr #5 !dbg !91 {
  call void @llvm.dbg.value(metadata double %0, metadata !93, metadata !DIExpression()), !dbg !95
  %2 = fptosi double %0 to i64, !dbg !96
  call void @llvm.dbg.value(metadata i64 %2, metadata !94, metadata !DIExpression()), !dbg !95
  %3 = inttoptr i64 %2 to i8*, !dbg !97
  %4 = load i8, i8* %3, align 1, !dbg !98, !tbaa !99
  %5 = sitofp i8 %4 to double, !dbg !98
  ret double %5, !dbg !100
}

; Function Attrs: nofree norecurse nounwind sspstrong uwtable writeonly
define double @cderefset(double, double) local_unnamed_addr #6 !dbg !101 {
  call void @llvm.dbg.value(metadata double %0, metadata !105, metadata !DIExpression()), !dbg !108
  call void @llvm.dbg.value(metadata double %1, metadata !106, metadata !DIExpression()), !dbg !108
  %3 = fptosi double %0 to i64, !dbg !109
  call void @llvm.dbg.value(metadata i64 %3, metadata !107, metadata !DIExpression()), !dbg !108
  %4 = fptosi double %1 to i8, !dbg !110
  %5 = inttoptr i64 %3 to i8*, !dbg !111
  store i8 %4, i8* %5, align 1, !dbg !112, !tbaa !99
  %6 = sitofp i8 %4 to double, !dbg !113
  ret double %6, !dbg !114
}

; Function Attrs: noreturn nounwind sspstrong uwtable
define i32 @main() local_unnamed_addr #7 !dbg !115 {
  %1 = alloca %struct.sresult, align 8
  %2 = alloca i64, align 8
  %3 = alloca i8*, align 8
  %4 = bitcast %struct.sresult* %1 to i8*, !dbg !139
  call void @llvm.lifetime.start.p0i8(i64 48, i8* nonnull %4) #9, !dbg !139
  call void @llvm.memset.p0i8.i64(i8* nonnull align 8 %4, i8 0, i64 48, i1 false), !dbg !140
  %5 = bitcast i64* %2 to i8*, !dbg !141
  call void @llvm.lifetime.start.p0i8(i64 8, i8* nonnull %5) #9, !dbg !141
  call void @llvm.dbg.value(metadata i64 1024, metadata !132, metadata !DIExpression()), !dbg !142
  store i64 1024, i64* %2, align 8, !dbg !143, !tbaa !144
  %6 = bitcast i8** %3 to i8*, !dbg !146
  call void @llvm.lifetime.start.p0i8(i64 8, i8* nonnull %6) #9, !dbg !146
  call void @llvm.dbg.value(metadata i64 1024, metadata !132, metadata !DIExpression()), !dbg !142
  %7 = tail call i8* @malloc(i64 1024), !dbg !147
  call void @llvm.dbg.value(metadata i8* %7, metadata !133, metadata !DIExpression()), !dbg !142
  store i8* %7, i8** %3, align 8, !dbg !148, !tbaa !39
  call void @llvm.dbg.value(metadata %struct.sresult* %1, metadata !119, metadata !DIExpression(DW_OP_deref)), !dbg !142
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.1, i64 0, i64 0), %struct.sresult* nonnull %1, i8* %7), !dbg !149
  %9 = getelementptr inbounds %struct.sresult, %struct.sresult* %1, i64 0, i32 3, !dbg !150
  %10 = getelementptr inbounds %struct.sresult, %struct.sresult* %1, i64 0, i32 1, !dbg !150
  %11 = getelementptr inbounds %struct.sresult, %struct.sresult* %1, i64 0, i32 0, !dbg !150
  %12 = getelementptr inbounds %struct.sresult, %struct.sresult* %1, i64 0, i32 5, !dbg !150
  %13 = getelementptr inbounds %struct.sresult, %struct.sresult* %1, i64 0, i32 2, !dbg !150
  %14 = getelementptr inbounds %struct.sresult, %struct.sresult* %1, i64 0, i32 4, !dbg !150
  br label %15, !dbg !151

15:                                               ; preds = %25, %0
  call void @llvm.dbg.value(metadata i32 -1, metadata !134, metadata !DIExpression()), !dbg !152
  %16 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.2, i64 0, i64 0)), !dbg !153
  %17 = load %struct._IO_FILE*, %struct._IO_FILE** @stdin, align 8, !dbg !154, !tbaa !39
  call void @llvm.dbg.value(metadata i64* %2, metadata !132, metadata !DIExpression(DW_OP_deref)), !dbg !142
  call void @llvm.dbg.value(metadata i8** %3, metadata !133, metadata !DIExpression(DW_OP_deref)), !dbg !142
  %18 = call i64 @getline(i8** nonnull %3, i64* nonnull %2, %struct._IO_FILE* %17) #9, !dbg !155
  call void @llvm.dbg.value(metadata i64 %18, metadata !136, metadata !DIExpression()), !dbg !152
  %19 = load i8*, i8** %3, align 8, !dbg !156, !tbaa !39
  call void @llvm.dbg.value(metadata i8* %19, metadata !133, metadata !DIExpression()), !dbg !142
  %20 = add i64 %18, -1, !dbg !157
  %21 = getelementptr inbounds i8, i8* %19, i64 %20, !dbg !156
  store i8 0, i8* %21, align 1, !dbg !158, !tbaa !99
  %22 = load i8*, i8** %3, align 8, !dbg !159, !tbaa !39
  call void @llvm.dbg.value(metadata i8* %22, metadata !133, metadata !DIExpression()), !dbg !142
  %23 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.3, i64 0, i64 0), i8* %22), !dbg !160
  %24 = load i8*, i8** %3, align 8, !dbg !161, !tbaa !39
  call void @llvm.dbg.value(metadata i8* %24, metadata !133, metadata !DIExpression()), !dbg !142
  call void @__nlex_feed(i8* %24) #9, !dbg !162
  br label %25, !dbg !163

25:                                               ; preds = %25, %15
  %26 = phi i32 [ -1, %15 ], [ %39, %25 ], !dbg !164
  call void @llvm.dbg.value(metadata i32 %26, metadata !134, metadata !DIExpression()), !dbg !152
  call void @llvm.dbg.value(metadata %struct.sresult* %1, metadata !119, metadata !DIExpression(DW_OP_deref)), !dbg !142
  call void @__nlex_root(%struct.sresult* nonnull %1) #9, !dbg !165
  %27 = load i8, i8* %9, align 8, !dbg !166, !tbaa !167
  %28 = icmp eq i8 %27, 0, !dbg !170
  %29 = select i1 %28, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str.6, i64 0, i64 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.5, i64 0, i64 0), !dbg !170
  %30 = load i32, i32* %10, align 8, !dbg !171, !tbaa !172
  %31 = load i8*, i8** %11, align 8, !dbg !173, !tbaa !174
  %32 = load i8*, i8** %12, align 8, !dbg !175, !tbaa !176
  %33 = load i8*, i8** %13, align 8, !dbg !177, !tbaa !178
  %34 = load i8, i8* %14, align 1, !dbg !179, !tbaa !180
  %35 = and i8 %34, 1, !dbg !181
  %36 = icmp eq i8 %35, 0, !dbg !182
  %37 = select i1 %36, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.8, i64 0, i64 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.7, i64 0, i64 0), !dbg !182
  %38 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([46 x i8], [46 x i8]* @.str.4, i64 0, i64 0), i8* %29, i32 %30, i8* %31, i8* %32, i32 %30, i8* %33, i8* %37), !dbg !183
  %39 = call i32 (...) @__nlex_distance() #9, !dbg !184
  call void @llvm.dbg.value(metadata i32 %39, metadata !137, metadata !DIExpression()), !dbg !150
  %40 = load i8, i8* %9, align 8, !dbg !185, !tbaa !167
  %41 = icmp ne i8 %40, 0, !dbg !187
  %42 = load i32, i32* %10, align 8, !dbg !188
  %43 = icmp eq i32 %42, 0, !dbg !189
  %44 = or i1 %41, %43, !dbg !190
  %45 = icmp eq i32 %39, %26, !dbg !191
  %46 = or i1 %45, %44, !dbg !190
  br i1 %46, label %15, label %25, !llvm.loop !192
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
!53 = distinct !DISubprogram(name: "mallocd", scope: !1, file: !1, line: 172, type: !21, scopeLine: 172, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !54)
!54 = !{!55, !56, !57, !58}
!55 = !DILocalVariable(name: "s", arg: 1, scope: !53, file: !1, line: 172, type: !5)
!56 = !DILocalVariable(name: "sze", scope: !53, file: !1, line: 173, type: !6)
!57 = !DILocalVariable(name: "mem", scope: !53, file: !1, line: 174, type: !12)
!58 = !DILocalVariable(name: "ip", scope: !53, file: !1, line: 175, type: !9)
!59 = !DILocation(line: 0, scope: !53)
!60 = !DILocation(line: 173, column: 16, scope: !53)
!61 = !DILocation(line: 174, column: 15, scope: !53)
!62 = !DILocation(line: 175, column: 19, scope: !53)
!63 = !DILocation(line: 176, column: 10, scope: !53)
!64 = !DILocation(line: 176, column: 3, scope: !53)
!65 = distinct !DISubprogram(name: "memsetd", scope: !1, file: !1, line: 179, type: !66, scopeLine: 179, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !68)
!66 = !DISubroutineType(types: !67)
!67 = !{!5, !5, !5, !5}
!68 = !{!69, !70, !71, !72}
!69 = !DILocalVariable(name: "p", arg: 1, scope: !65, file: !1, line: 179, type: !5)
!70 = !DILocalVariable(name: "s", arg: 2, scope: !65, file: !1, line: 179, type: !5)
!71 = !DILocalVariable(name: "n", arg: 3, scope: !65, file: !1, line: 179, type: !5)
!72 = !DILocalVariable(name: "ip", scope: !65, file: !1, line: 180, type: !9)
!73 = !DILocation(line: 0, scope: !65)
!74 = !DILocation(line: 180, column: 19, scope: !65)
!75 = !DILocation(line: 181, column: 28, scope: !65)
!76 = !DILocation(line: 181, column: 40, scope: !65)
!77 = !DILocation(line: 181, column: 21, scope: !65)
!78 = !DILocation(line: 181, column: 49, scope: !65)
!79 = !DILocation(line: 182, column: 10, scope: !65)
!80 = !DILocation(line: 182, column: 3, scope: !65)
!81 = distinct !DISubprogram(name: "freed", scope: !1, file: !1, line: 185, type: !21, scopeLine: 185, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !82)
!82 = !{!83, !84}
!83 = !DILocalVariable(name: "ptr", arg: 1, scope: !81, file: !1, line: 185, type: !5)
!84 = !DILocalVariable(name: "p", scope: !81, file: !1, line: 186, type: !9)
!85 = !DILocation(line: 0, scope: !81)
!86 = !DILocation(line: 186, column: 18, scope: !81)
!87 = !DILocation(line: 187, column: 24, scope: !81)
!88 = !DILocation(line: 187, column: 19, scope: !81)
!89 = !DILocation(line: 187, column: 10, scope: !81)
!90 = !DILocation(line: 187, column: 3, scope: !81)
!91 = distinct !DISubprogram(name: "cderef", scope: !1, file: !1, line: 190, type: !21, scopeLine: 190, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !92)
!92 = !{!93, !94}
!93 = !DILocalVariable(name: "ptr", arg: 1, scope: !91, file: !1, line: 190, type: !5)
!94 = !DILocalVariable(name: "p", scope: !91, file: !1, line: 191, type: !9)
!95 = !DILocation(line: 0, scope: !91)
!96 = !DILocation(line: 191, column: 18, scope: !91)
!97 = !DILocation(line: 192, column: 13, scope: !91)
!98 = !DILocation(line: 192, column: 10, scope: !91)
!99 = !{!41, !41, i64 0}
!100 = !DILocation(line: 192, column: 3, scope: !91)
!101 = distinct !DISubprogram(name: "cderefset", scope: !1, file: !1, line: 195, type: !102, scopeLine: 195, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !104)
!102 = !DISubroutineType(types: !103)
!103 = !{!5, !5, !5}
!104 = !{!105, !106, !107}
!105 = !DILocalVariable(name: "ptr", arg: 1, scope: !101, file: !1, line: 195, type: !5)
!106 = !DILocalVariable(name: "value", arg: 2, scope: !101, file: !1, line: 195, type: !5)
!107 = !DILocalVariable(name: "p", scope: !101, file: !1, line: 196, type: !9)
!108 = !DILocation(line: 0, scope: !101)
!109 = !DILocation(line: 196, column: 18, scope: !101)
!110 = !DILocation(line: 197, column: 35, scope: !101)
!111 = !DILocation(line: 197, column: 22, scope: !101)
!112 = !DILocation(line: 197, column: 33, scope: !101)
!113 = !DILocation(line: 197, column: 10, scope: !101)
!114 = !DILocation(line: 197, column: 3, scope: !101)
!115 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 201, type: !116, scopeLine: 201, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !118)
!116 = !DISubroutineType(types: !117)
!117 = !{!13}
!118 = !{!119, !132, !133, !134, !136, !137}
!119 = !DILocalVariable(name: "res", scope: !115, file: !1, line: 203, type: !120)
!120 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "sresult", file: !1, line: 4, size: 384, elements: !121)
!121 = !{!122, !125, !126, !127, !128, !130, !131}
!122 = !DIDerivedType(tag: DW_TAG_member, name: "start", scope: !120, file: !1, line: 5, baseType: !123, size: 64)
!123 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !124, size: 64)
!124 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !4)
!125 = !DIDerivedType(tag: DW_TAG_member, name: "length", scope: !120, file: !1, line: 6, baseType: !13, size: 32, offset: 64)
!126 = !DIDerivedType(tag: DW_TAG_member, name: "tag", scope: !120, file: !1, line: 7, baseType: !123, size: 64, offset: 128)
!127 = !DIDerivedType(tag: DW_TAG_member, name: "errc", scope: !120, file: !1, line: 8, baseType: !4, size: 8, offset: 192)
!128 = !DIDerivedType(tag: DW_TAG_member, name: "metadata", scope: !120, file: !1, line: 9, baseType: !129, size: 8, offset: 200)
!129 = !DIBasicType(name: "unsigned char", size: 8, encoding: DW_ATE_unsigned_char)
!130 = !DIDerivedType(tag: DW_TAG_member, name: "pos", scope: !120, file: !1, line: 10, baseType: !123, size: 64, offset: 256)
!131 = !DIDerivedType(tag: DW_TAG_member, name: "allocd", scope: !120, file: !1, line: 11, baseType: !13, size: 32, offset: 320)
!132 = !DILocalVariable(name: "size", scope: !115, file: !1, line: 204, type: !6)
!133 = !DILocalVariable(name: "s", scope: !115, file: !1, line: 205, type: !14)
!134 = !DILocalVariable(name: "last", scope: !135, file: !1, line: 208, type: !13)
!135 = distinct !DILexicalBlock(scope: !115, file: !1, line: 207, column: 13)
!136 = !DILocalVariable(name: "els", scope: !135, file: !1, line: 210, type: !6)
!137 = !DILocalVariable(name: "dist", scope: !138, file: !1, line: 222, type: !13)
!138 = distinct !DILexicalBlock(scope: !135, file: !1, line: 215, column: 48)
!139 = !DILocation(line: 203, column: 3, scope: !115)
!140 = !DILocation(line: 203, column: 18, scope: !115)
!141 = !DILocation(line: 204, column: 3, scope: !115)
!142 = !DILocation(line: 0, scope: !115)
!143 = !DILocation(line: 204, column: 10, scope: !115)
!144 = !{!145, !145, i64 0}
!145 = !{!"long", !41, i64 0}
!146 = !DILocation(line: 205, column: 3, scope: !115)
!147 = !DILocation(line: 205, column: 13, scope: !115)
!148 = !DILocation(line: 205, column: 9, scope: !115)
!149 = !DILocation(line: 206, column: 3, scope: !115)
!150 = !DILocation(line: 0, scope: !138)
!151 = !DILocation(line: 207, column: 3, scope: !115)
!152 = !DILocation(line: 0, scope: !135)
!153 = !DILocation(line: 209, column: 5, scope: !135)
!154 = !DILocation(line: 210, column: 37, scope: !135)
!155 = !DILocation(line: 210, column: 18, scope: !135)
!156 = !DILocation(line: 211, column: 5, scope: !135)
!157 = !DILocation(line: 211, column: 11, scope: !135)
!158 = !DILocation(line: 211, column: 16, scope: !135)
!159 = !DILocation(line: 212, column: 35, scope: !135)
!160 = !DILocation(line: 212, column: 5, scope: !135)
!161 = !DILocation(line: 214, column: 17, scope: !135)
!162 = !DILocation(line: 214, column: 5, scope: !135)
!163 = !DILocation(line: 215, column: 5, scope: !135)
!164 = !DILocation(line: 208, column: 9, scope: !135)
!165 = !DILocation(line: 217, column: 7, scope: !138)
!166 = !DILocation(line: 220, column: 19, scope: !138)
!167 = !{!168, !41, i64 24}
!168 = !{!"sresult", !40, i64 0, !169, i64 8, !40, i64 16, !41, i64 24, !41, i64 25, !40, i64 32, !169, i64 40}
!169 = !{!"int", !41, i64 0}
!170 = !DILocation(line: 220, column: 15, scope: !138)
!171 = !DILocation(line: 220, column: 43, scope: !138)
!172 = !{!168, !169, i64 8}
!173 = !DILocation(line: 220, column: 55, scope: !138)
!174 = !{!168, !40, i64 0}
!175 = !DILocation(line: 220, column: 66, scope: !138)
!176 = !{!168, !40, i64 32}
!177 = !DILocation(line: 221, column: 30, scope: !138)
!178 = !{!168, !40, i64 16}
!179 = !DILocation(line: 221, column: 40, scope: !138)
!180 = !{!168, !41, i64 25}
!181 = !DILocation(line: 221, column: 49, scope: !138)
!182 = !DILocation(line: 221, column: 36, scope: !138)
!183 = !DILocation(line: 219, column: 7, scope: !138)
!184 = !DILocation(line: 222, column: 18, scope: !138)
!185 = !DILocation(line: 223, column: 15, scope: !186)
!186 = distinct !DILexicalBlock(scope: !138, file: !1, line: 223, column: 11)
!187 = !DILocation(line: 223, column: 11, scope: !186)
!188 = !DILocation(line: 223, column: 27, scope: !186)
!189 = !DILocation(line: 223, column: 34, scope: !186)
!190 = !DILocation(line: 223, column: 20, scope: !186)
!191 = !DILocation(line: 223, column: 47, scope: !186)
!192 = distinct !{!192, !151, !193}
!193 = !DILocation(line: 227, column: 3, scope: !115)
