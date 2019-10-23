; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i64, i16, i8, [1 x i8], i8*, i64, %struct._IO_codecvt*, %struct._IO_wide_data*, %struct._IO_FILE*, i8*, i64, i32, [20 x i8] }
%struct._IO_marker = type opaque
%struct._IO_codecvt = type opaque
%struct._IO_wide_data = type opaque
%struct.sresult = type { i8*, i32, i8*, i8, i8 }

@.str = private unnamed_addr constant [20 x i8] c"res at %p, s at %p\0A\00", align 1
@.str.1 = private unnamed_addr constant [3 x i8] c"> \00", align 1
@stdin = external local_unnamed_addr global %struct._IO_FILE*, align 8
@.str.2 = private unnamed_addr constant [19 x i8] c"processing - '%s'\0A\00", align 1
@.str.3 = private unnamed_addr constant [39 x i8] c"%smatch {'%.*s' %d %s} is%sa stopword\0A\00", align 1
@.str.4 = private unnamed_addr constant [4 x i8] c"no \00", align 1
@.str.5 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@.str.6 = private unnamed_addr constant [2 x i8] c" \00", align 1
@.str.7 = private unnamed_addr constant [6 x i8] c" not \00", align 1

; Function Attrs: noreturn nounwind sspstrong uwtable
define dso_local i32 @main() local_unnamed_addr #0 !dbg !9 {
  %1 = alloca %struct.sresult, align 8
  %2 = alloca i64, align 8
  %3 = alloca i8*, align 8
  %4 = bitcast %struct.sresult* %1 to i8*, !dbg !35
  call void @llvm.lifetime.start.p0i8(i64 32, i8* nonnull %4) #5, !dbg !35
  call void @llvm.memset.p0i8.i64(i8* nonnull align 8 %4, i8 0, i64 32, i1 false), !dbg !36
  %5 = bitcast i64* %2 to i8*, !dbg !37
  call void @llvm.lifetime.start.p0i8(i64 8, i8* nonnull %5) #5, !dbg !37
  call void @llvm.dbg.value(metadata i64 1024, metadata !26, metadata !DIExpression()), !dbg !38
  store i64 1024, i64* %2, align 8, !dbg !39, !tbaa !40
  %6 = bitcast i8** %3 to i8*, !dbg !44
  call void @llvm.lifetime.start.p0i8(i64 8, i8* nonnull %6) #5, !dbg !44
  call void @llvm.dbg.value(metadata i64 1024, metadata !26, metadata !DIExpression()), !dbg !38
  %7 = tail call i8* @malloc(i64 1024), !dbg !45
  call void @llvm.dbg.value(metadata i8* %7, metadata !30, metadata !DIExpression()), !dbg !38
  store i8* %7, i8** %3, align 8, !dbg !46, !tbaa !47
  call void @llvm.dbg.value(metadata %struct.sresult* %1, metadata !14, metadata !DIExpression(DW_OP_deref)), !dbg !38
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str, i64 0, i64 0), %struct.sresult* nonnull %1, i8* %7), !dbg !49
  %9 = getelementptr inbounds %struct.sresult, %struct.sresult* %1, i64 0, i32 3, !dbg !50
  %10 = getelementptr inbounds %struct.sresult, %struct.sresult* %1, i64 0, i32 1, !dbg !50
  %11 = getelementptr inbounds %struct.sresult, %struct.sresult* %1, i64 0, i32 0, !dbg !50
  %12 = getelementptr inbounds %struct.sresult, %struct.sresult* %1, i64 0, i32 2, !dbg !50
  %13 = getelementptr inbounds %struct.sresult, %struct.sresult* %1, i64 0, i32 4, !dbg !50
  br label %14, !dbg !52

14:                                               ; preds = %24, %0
  call void @llvm.dbg.value(metadata i8* inttoptr (i64 -1 to i8*), metadata !32, metadata !DIExpression()), !dbg !53
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i64 0, i64 0)), !dbg !54
  %16 = load %struct._IO_FILE*, %struct._IO_FILE** @stdin, align 8, !dbg !55, !tbaa !47
  call void @llvm.dbg.value(metadata i64* %2, metadata !26, metadata !DIExpression(DW_OP_deref)), !dbg !38
  call void @llvm.dbg.value(metadata i8** %3, metadata !30, metadata !DIExpression(DW_OP_deref)), !dbg !38
  %17 = call i64 @getline(i8** nonnull %3, i64* nonnull %2, %struct._IO_FILE* %16) #5, !dbg !56
  call void @llvm.dbg.value(metadata i64 %17, metadata !34, metadata !DIExpression()), !dbg !53
  %18 = load i8*, i8** %3, align 8, !dbg !57, !tbaa !47
  call void @llvm.dbg.value(metadata i8* %18, metadata !30, metadata !DIExpression()), !dbg !38
  %19 = add i64 %17, -1, !dbg !58
  %20 = getelementptr inbounds i8, i8* %18, i64 %19, !dbg !57
  store i8 0, i8* %20, align 1, !dbg !59, !tbaa !60
  %21 = load i8*, i8** %3, align 8, !dbg !61, !tbaa !47
  call void @llvm.dbg.value(metadata i8* %21, metadata !30, metadata !DIExpression()), !dbg !38
  %22 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.2, i64 0, i64 0), i8* %21), !dbg !62
  %23 = load i8*, i8** %3, align 8, !dbg !63, !tbaa !47
  call void @llvm.dbg.value(metadata i8* %23, metadata !30, metadata !DIExpression()), !dbg !38
  call void @__nlex_feed(i8* %23) #5, !dbg !64
  br label %24, !dbg !65

24:                                               ; preds = %24, %14
  call void @llvm.dbg.value(metadata %struct.sresult* %1, metadata !14, metadata !DIExpression(DW_OP_deref)), !dbg !38
  call void @__nlex_root(%struct.sresult* nonnull %1) #5, !dbg !66
  %25 = load i8, i8* %9, align 8, !dbg !67, !tbaa !68
  %26 = icmp eq i8 %25, 0, !dbg !71
  %27 = select i1 %26, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str.5, i64 0, i64 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.4, i64 0, i64 0), !dbg !71
  %28 = load i32, i32* %10, align 8, !dbg !72, !tbaa !73
  %29 = load i8*, i8** %11, align 8, !dbg !74, !tbaa !75
  %30 = load i8*, i8** %12, align 8, !dbg !76, !tbaa !77
  %31 = load i8, i8* %13, align 1, !dbg !78, !tbaa !79
  %32 = and i8 %31, 1, !dbg !80
  %33 = icmp eq i8 %32, 0, !dbg !81
  %34 = select i1 %33, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.7, i64 0, i64 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.6, i64 0, i64 0), !dbg !81
  %35 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([39 x i8], [39 x i8]* @.str.3, i64 0, i64 0), i8* %27, i32 %28, i8* %29, i32 %28, i8* %30, i8* %34), !dbg !82
  %36 = load i8, i8* %9, align 8, !dbg !83, !tbaa !68
  %37 = icmp ne i8 %36, 0, !dbg !85
  %38 = load i32, i32* %10, align 8, !dbg !86
  %39 = icmp eq i32 %38, 0, !dbg !87
  %40 = or i1 %37, %39, !dbg !88
  br i1 %40, label %14, label %24, !dbg !88, !llvm.loop !89
}

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #1

; Function Attrs: nofree nounwind
declare noalias i8* @malloc(i64) local_unnamed_addr #2

; Function Attrs: nofree nounwind
declare i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #2

declare i64 @getline(i8**, i64*, %struct._IO_FILE*) local_unnamed_addr #3

declare void @__nlex_feed(i8*) local_unnamed_addr #3

declare void @__nlex_root(%struct.sresult*) local_unnamed_addr #3

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.value(metadata, metadata, metadata) #4

attributes #0 = { noreturn nounwind sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind }
attributes #2 = { nofree nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind readnone speculatable }
attributes #5 = { nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4, !5, !6, !7}
!llvm.ident = !{!8}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 9.0.0 (tags/RELEASE_900/final)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, nameTableKind: None)
!1 = !DIFile(filename: "test.c", directory: "/home/Test/Documents/nlp-lex/src/playground")
!2 = !{}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{i32 7, !"PIC Level", i32 2}
!7 = !{i32 7, !"PIE Level", i32 2}
!8 = !{!"clang version 9.0.0 (tags/RELEASE_900/final)"}
!9 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 13, type: !10, scopeLine: 13, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !13)
!10 = !DISubroutineType(types: !11)
!11 = !{!12}
!12 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!13 = !{!14, !26, !30, !32, !34}
!14 = !DILocalVariable(name: "res", scope: !9, file: !1, line: 14, type: !15)
!15 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "sresult", file: !1, line: 3, size: 256, elements: !16)
!16 = !{!17, !21, !22, !23, !24}
!17 = !DIDerivedType(tag: DW_TAG_member, name: "start", scope: !15, file: !1, line: 4, baseType: !18, size: 64)
!18 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !19, size: 64)
!19 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !20)
!20 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!21 = !DIDerivedType(tag: DW_TAG_member, name: "length", scope: !15, file: !1, line: 5, baseType: !12, size: 32, offset: 64)
!22 = !DIDerivedType(tag: DW_TAG_member, name: "tag", scope: !15, file: !1, line: 6, baseType: !18, size: 64, offset: 128)
!23 = !DIDerivedType(tag: DW_TAG_member, name: "errc", scope: !15, file: !1, line: 7, baseType: !20, size: 8, offset: 192)
!24 = !DIDerivedType(tag: DW_TAG_member, name: "metadata", scope: !15, file: !1, line: 8, baseType: !25, size: 8, offset: 200)
!25 = !DIBasicType(name: "unsigned char", size: 8, encoding: DW_ATE_unsigned_char)
!26 = !DILocalVariable(name: "size", scope: !9, file: !1, line: 15, type: !27)
!27 = !DIDerivedType(tag: DW_TAG_typedef, name: "size_t", file: !28, line: 46, baseType: !29)
!28 = !DIFile(filename: "/usr/lib/clang/9.0.0/include/stddef.h", directory: "")
!29 = !DIBasicType(name: "long unsigned int", size: 64, encoding: DW_ATE_unsigned)
!30 = !DILocalVariable(name: "s", scope: !9, file: !1, line: 16, type: !31)
!31 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !20, size: 64)
!32 = !DILocalVariable(name: "last", scope: !33, file: !1, line: 19, type: !31)
!33 = distinct !DILexicalBlock(scope: !9, file: !1, line: 18, column: 14)
!34 = !DILocalVariable(name: "els", scope: !33, file: !1, line: 21, type: !27)
!35 = !DILocation(line: 14, column: 5, scope: !9)
!36 = !DILocation(line: 14, column: 20, scope: !9)
!37 = !DILocation(line: 15, column: 5, scope: !9)
!38 = !DILocation(line: 0, scope: !9)
!39 = !DILocation(line: 15, column: 12, scope: !9)
!40 = !{!41, !41, i64 0}
!41 = !{!"long", !42, i64 0}
!42 = !{!"omnipotent char", !43, i64 0}
!43 = !{!"Simple C/C++ TBAA"}
!44 = !DILocation(line: 16, column: 5, scope: !9)
!45 = !DILocation(line: 16, column: 15, scope: !9)
!46 = !DILocation(line: 16, column: 11, scope: !9)
!47 = !{!48, !48, i64 0}
!48 = !{!"any pointer", !42, i64 0}
!49 = !DILocation(line: 17, column: 5, scope: !9)
!50 = !DILocation(line: 0, scope: !51)
!51 = distinct !DILexicalBlock(scope: !33, file: !1, line: 25, column: 17)
!52 = !DILocation(line: 18, column: 5, scope: !9)
!53 = !DILocation(line: 0, scope: !33)
!54 = !DILocation(line: 20, column: 7, scope: !33)
!55 = !DILocation(line: 21, column: 39, scope: !33)
!56 = !DILocation(line: 21, column: 20, scope: !33)
!57 = !DILocation(line: 22, column: 7, scope: !33)
!58 = !DILocation(line: 22, column: 12, scope: !33)
!59 = !DILocation(line: 22, column: 16, scope: !33)
!60 = !{!42, !42, i64 0}
!61 = !DILocation(line: 23, column: 37, scope: !33)
!62 = !DILocation(line: 23, column: 7, scope: !33)
!63 = !DILocation(line: 24, column: 19, scope: !33)
!64 = !DILocation(line: 24, column: 7, scope: !33)
!65 = !DILocation(line: 25, column: 7, scope: !33)
!66 = !DILocation(line: 26, column: 9, scope: !51)
!67 = !DILocation(line: 27, column: 64, scope: !51)
!68 = !{!69, !42, i64 24}
!69 = !{!"sresult", !48, i64 0, !70, i64 8, !48, i64 16, !42, i64 24, !42, i64 25}
!70 = !{!"int", !42, i64 0}
!71 = !DILocation(line: 27, column: 60, scope: !51)
!72 = !DILocation(line: 27, column: 84, scope: !51)
!73 = !{!69, !70, i64 8}
!74 = !DILocation(line: 27, column: 96, scope: !51)
!75 = !{!69, !48, i64 0}
!76 = !DILocation(line: 27, column: 119, scope: !51)
!77 = !{!69, !48, i64 16}
!78 = !DILocation(line: 27, column: 129, scope: !51)
!79 = !{!69, !42, i64 25}
!80 = !DILocation(line: 27, column: 137, scope: !51)
!81 = !DILocation(line: 27, column: 125, scope: !51)
!82 = !DILocation(line: 27, column: 9, scope: !51)
!83 = !DILocation(line: 28, column: 17, scope: !84)
!84 = distinct !DILexicalBlock(scope: !51, file: !1, line: 28, column: 13)
!85 = !DILocation(line: 28, column: 13, scope: !84)
!86 = !DILocation(line: 28, column: 29, scope: !84)
!87 = !DILocation(line: 28, column: 36, scope: !84)
!88 = !DILocation(line: 28, column: 22, scope: !84)
!89 = distinct !{!89, !52, !90}
!90 = !DILocation(line: 32, column: 5, scope: !9)
