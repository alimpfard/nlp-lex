%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i64, i16, i8, [1 x i8], i8*, i64, %struct._IO_codecvt*, %struct._IO_wide_data*, %struct._IO_FILE*, i8*, i64, i32, [20 x i8] }
%struct._IO_marker = type opaque
%struct._IO_codecvt = type opaque
%struct._IO_wide_data = type opaque

@.str = private unnamed_addr constant [20 x i8] c"res at %p, s at %p\0A\00", align 1
@.str.1 = private unnamed_addr constant [3 x i8] c"> \00", align 1
@stdin = external local_unnamed_addr global %struct._IO_FILE*, align 8
@.str.2 = private unnamed_addr constant [19 x i8] c"processing - '%s'\0A\00", align 1
@.str.3 = private unnamed_addr constant [24 x i8] c"%smatch {'%.*s' %d %s}\0A\00", align 1
@.str.4 = private unnamed_addr constant [4 x i8] c"no \00", align 1
@.str.5 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1

; Function Attrs: noreturn nounwind sspstrong uwtable
define dso_local i32 @main() local_unnamed_addr #0 !dbg !9 {
  %1 = alloca %0, align 8
  %2 = alloca i64, align 8
  %3 = alloca i8*, align 8
  %4 = bitcast %0* %1 to i8*, !dbg !33
  call void @llvm.lifetime.start.p0i8(i64 32, i8* nonnull %4) #5, !dbg !33
  call void @llvm.memset.p0i8.i64(i8* nonnull align 8 %4, i8 0, i64 32, i1 false), !dbg !34
  %5 = bitcast i64* %2 to i8*, !dbg !35
  call void @llvm.lifetime.start.p0i8(i64 8, i8* nonnull %5) #5, !dbg !35
  call void @llvm.dbg.value(metadata i64 1024, metadata !24, metadata !DIExpression()), !dbg !36
  store i64 1024, i64* %2, align 8, !dbg !36, !tbaa !37
  %6 = bitcast i8** %3 to i8*, !dbg !41
  call void @llvm.lifetime.start.p0i8(i64 8, i8* nonnull %6) #5, !dbg !41
  call void @llvm.dbg.value(metadata i64 1024, metadata !24, metadata !DIExpression()), !dbg !36
  %7 = tail call i8* @malloc(i64 1024), !dbg !42
  call void @llvm.dbg.value(metadata i8* %7, metadata !28, metadata !DIExpression()), !dbg !43
  store i8* %7, i8** %3, align 8, !dbg !43, !tbaa !44
  call void @llvm.dbg.value(metadata %0* %1, metadata !14, metadata !DIExpression(DW_OP_deref)), !dbg !34
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str, i64 0, i64 0), %0* nonnull %1, i8* %7), !dbg !46
  %9 = getelementptr inbounds %0, %0* %1, i64 0, i32 3
  %10 = getelementptr inbounds %0, %0* %1, i64 0, i32 1
  %11 = getelementptr inbounds %0, %0* %1, i64 0, i32 0
  %12 = getelementptr inbounds %0, %0* %1, i64 0, i32 2
  br label %13, !dbg !47

; <label>:13:                                     ; preds = %23, %0
  call void @llvm.dbg.value(metadata i8* inttoptr (i64 -1 to i8*), metadata !30, metadata !DIExpression()), !dbg !48
  %14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i64 0, i64 0)), !dbg !49
  %15 = load %struct._IO_FILE*, %struct._IO_FILE** @stdin, align 8, !dbg !50, !tbaa !44
  call void @llvm.dbg.value(metadata i64* %2, metadata !24, metadata !DIExpression(DW_OP_deref)), !dbg !36
  call void @llvm.dbg.value(metadata i8** %3, metadata !28, metadata !DIExpression(DW_OP_deref)), !dbg !43
  %16 = call i64 @getline(i8** nonnull %3, i64* nonnull %2, %struct._IO_FILE* %15) #5, !dbg !51
  call void @llvm.dbg.value(metadata i64 %16, metadata !32, metadata !DIExpression()), !dbg !52
  %17 = load i8*, i8** %3, align 8, !dbg !53, !tbaa !44
  call void @llvm.dbg.value(metadata i8* %17, metadata !28, metadata !DIExpression()), !dbg !43
  %18 = add i64 %16, -1, !dbg !54
  %19 = getelementptr inbounds i8, i8* %17, i64 %18, !dbg !53
  store i8 0, i8* %19, align 1, !dbg !55, !tbaa !56
  %20 = load i8*, i8** %3, align 8, !dbg !57, !tbaa !44
  call void @llvm.dbg.value(metadata i8* %20, metadata !28, metadata !DIExpression()), !dbg !43
  %21 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.2, i64 0, i64 0), i8* %20), !dbg !58
  %22 = load i8*, i8** %3, align 8, !dbg !59, !tbaa !44
  call void @llvm.dbg.value(metadata i8* %22, metadata !28, metadata !DIExpression()), !dbg !43
  call void @__nlex_feed(i8* %22) #5, !dbg !60
  br label %23, !dbg !61

; <label>:23:                                     ; preds = %23, %13
  call void @llvm.dbg.value(metadata %0* %1, metadata !14, metadata !DIExpression(DW_OP_deref)), !dbg !34
  call void @__nlex_root(%0* nonnull %1) #5, !dbg !62
  %24 = load i8, i8* %9, align 8, !dbg !64, !tbaa !65
  %25 = icmp eq i8 %24, 0, !dbg !68
  %26 = select i1 %25, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str.5, i64 0, i64 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.4, i64 0, i64 0), !dbg !68
  %27 = load i32, i32* %10, align 8, !dbg !69, !tbaa !70
  %28 = load i8*, i8** %11, align 8, !dbg !71, !tbaa !72
  %29 = load i8*, i8** %12, align 8, !dbg !73, !tbaa !74
  %30 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str.3, i64 0, i64 0), i8* %26, i32 %27, i8* %28, i32 %27, i8* %29), !dbg !75
  %31 = load i8, i8* %9, align 8, !dbg !76, !tbaa !65
  %32 = icmp ne i8 %31, 0, !dbg !78
  %33 = load i32, i32* %10, align 8, !dbg !79
  %34 = icmp eq i32 %33, 0, !dbg !80
  %35 = or i1 %32, %34, !dbg !81
  br i1 %35, label %13, label %23, !dbg !81, !llvm.loop !82
}

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start.p0i8(i64, i8* nocapture) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1) #1

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) local_unnamed_addr #2

; Function Attrs: nounwind
declare i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #2

declare i64 @getline(i8**, i64*, %struct._IO_FILE*) local_unnamed_addr #3

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.value(metadata, metadata, metadata) #4

attributes #0 = { noreturn nounwind sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind readnone speculatable }
attributes #5 = { nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4, !5, !6, !7}
!llvm.ident = !{!8}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 8.0.1 (tags/RELEASE_801/final)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, nameTableKind: None)
!1 = !DIFile(filename: "test.c", directory: "/home/Test/Documents/nlp-lex/src/playground")
!2 = !{}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{i32 7, !"PIC Level", i32 2}
!7 = !{i32 7, !"PIE Level", i32 2}
!8 = !{!"clang version 8.0.1 (tags/RELEASE_801/final)"}
!9 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 12, type: !10, scopeLine: 12, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !13)
!10 = !DISubroutineType(types: !11)
!11 = !{!12}
!12 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!13 = !{!14, !24, !28, !30, !32}
!14 = !DILocalVariable(name: "res", scope: !9, file: !1, line: 13, type: !15)
!15 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "sresult", file: !1, line: 3, size: 256, elements: !16)
!16 = !{!17, !21, !22, !23}
!17 = !DIDerivedType(tag: DW_TAG_member, name: "start", scope: !15, file: !1, line: 4, baseType: !18, size: 64)
!18 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !19, size: 64)
!19 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !20)
!20 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!21 = !DIDerivedType(tag: DW_TAG_member, name: "length", scope: !15, file: !1, line: 5, baseType: !12, size: 32, offset: 64)
!22 = !DIDerivedType(tag: DW_TAG_member, name: "tag", scope: !15, file: !1, line: 6, baseType: !18, size: 64, offset: 128)
!23 = !DIDerivedType(tag: DW_TAG_member, name: "errc", scope: !15, file: !1, line: 7, baseType: !20, size: 8, offset: 192)
!24 = !DILocalVariable(name: "size", scope: !9, file: !1, line: 14, type: !25)
!25 = !DIDerivedType(tag: DW_TAG_typedef, name: "size_t", file: !26, line: 62, baseType: !27)
!26 = !DIFile(filename: "/usr/lib/clang/8.0.1/include/stddef.h", directory: "")
!27 = !DIBasicType(name: "long unsigned int", size: 64, encoding: DW_ATE_unsigned)
!28 = !DILocalVariable(name: "s", scope: !9, file: !1, line: 15, type: !29)
!29 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !20, size: 64)
!30 = !DILocalVariable(name: "last", scope: !31, file: !1, line: 18, type: !29)
!31 = distinct !DILexicalBlock(scope: !9, file: !1, line: 17, column: 14)
!32 = !DILocalVariable(name: "els", scope: !31, file: !1, line: 20, type: !25)
!33 = !DILocation(line: 13, column: 5, scope: !9)
!34 = !DILocation(line: 13, column: 20, scope: !9)
!35 = !DILocation(line: 14, column: 5, scope: !9)
!36 = !DILocation(line: 14, column: 12, scope: !9)
!37 = !{!38, !38, i64 0}
!38 = !{!"long", !39, i64 0}
!39 = !{!"omnipotent char", !40, i64 0}
!40 = !{!"Simple C/C++ TBAA"}
!41 = !DILocation(line: 15, column: 5, scope: !9)
!42 = !DILocation(line: 15, column: 15, scope: !9)
!43 = !DILocation(line: 15, column: 11, scope: !9)
!44 = !{!45, !45, i64 0}
!45 = !{!"any pointer", !39, i64 0}
!46 = !DILocation(line: 16, column: 5, scope: !9)
!47 = !DILocation(line: 17, column: 5, scope: !9)
!48 = !DILocation(line: 18, column: 13, scope: !31)
!49 = !DILocation(line: 19, column: 7, scope: !31)
!50 = !DILocation(line: 20, column: 39, scope: !31)
!51 = !DILocation(line: 20, column: 20, scope: !31)
!52 = !DILocation(line: 20, column: 14, scope: !31)
!53 = !DILocation(line: 21, column: 7, scope: !31)
!54 = !DILocation(line: 21, column: 12, scope: !31)
!55 = !DILocation(line: 21, column: 16, scope: !31)
!56 = !{!39, !39, i64 0}
!57 = !DILocation(line: 22, column: 37, scope: !31)
!58 = !DILocation(line: 22, column: 7, scope: !31)
!59 = !DILocation(line: 23, column: 19, scope: !31)
!60 = !DILocation(line: 23, column: 7, scope: !31)
!61 = !DILocation(line: 24, column: 7, scope: !31)
!62 = !DILocation(line: 25, column: 9, scope: !63)
!63 = distinct !DILexicalBlock(scope: !31, file: !1, line: 24, column: 17)
!64 = !DILocation(line: 26, column: 49, scope: !63)
!65 = !{!66, !39, i64 24}
!66 = !{!"sresult", !45, i64 0, !67, i64 8, !45, i64 16, !39, i64 24}
!67 = !{!"int", !39, i64 0}
!68 = !DILocation(line: 26, column: 45, scope: !63)
!69 = !DILocation(line: 26, column: 69, scope: !63)
!70 = !{!66, !67, i64 8}
!71 = !DILocation(line: 26, column: 81, scope: !63)
!72 = !{!66, !45, i64 0}
!73 = !DILocation(line: 26, column: 104, scope: !63)
!74 = !{!66, !45, i64 16}
!75 = !DILocation(line: 26, column: 9, scope: !63)
!76 = !DILocation(line: 27, column: 17, scope: !77)
!77 = distinct !DILexicalBlock(scope: !63, file: !1, line: 27, column: 13)
!78 = !DILocation(line: 27, column: 13, scope: !77)
!79 = !DILocation(line: 27, column: 29, scope: !77)
!80 = !DILocation(line: 27, column: 36, scope: !77)
!81 = !DILocation(line: 27, column: 22, scope: !77)
!82 = distinct !{!82, !47, !83}
!83 = !DILocation(line: 31, column: 5, scope: !9)