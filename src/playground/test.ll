; ModuleID = 'test.c'
%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i64, i16, i8, [1 x i8], i8*, i64, %struct._IO_codecvt*, %struct._IO_wide_data*, %struct._IO_FILE*, i8*, i64, i32, [20 x i8] }
%struct._IO_marker = type opaque
%struct._IO_codecvt = type opaque
%struct._IO_wide_data = type opaque

@.str = private unnamed_addr constant [20 x i8] c"res at %p, s at %p\0A\00", align 1
@.str.1 = private unnamed_addr constant [3 x i8] c"> \00", align 1
@stdin = external local_unnamed_addr global %struct._IO_FILE*, align 8
@.str.2 = private unnamed_addr constant [19 x i8] c"processing - '%s'\0A\00", align 1
@.str.3 = private unnamed_addr constant [14 x i8] c"'%.*s' %d %s\0A\00", align 1

; Function Attrs: noreturn nounwind sspstrong uwtable
define dso_local i32 @main() local_unnamed_addr #0 !dbg !9 {
  %1 = alloca %0, align 8
  %2 = alloca i64, align 8
  %3 = alloca i8*, align 8
  %4 = bitcast %0* %1 to i8*, !dbg !32
  call void @llvm.lifetime.start.p0i8(i64 24, i8* nonnull %4) #5, !dbg !32
  call void @llvm.memset.p0i8.i64(i8* nonnull align 8 %4, i8 0, i64 24, i1 false), !dbg !33
  %5 = bitcast i64* %2 to i8*, !dbg !34
  call void @llvm.lifetime.start.p0i8(i64 8, i8* nonnull %5) #5, !dbg !34
  call void @llvm.dbg.value(metadata i64 1024, metadata !23, metadata !DIExpression()), !dbg !35
  store i64 1024, i64* %2, align 8, !dbg !35, !tbaa !36
  %6 = bitcast i8** %3 to i8*, !dbg !40
  call void @llvm.lifetime.start.p0i8(i64 8, i8* nonnull %6) #5, !dbg !40
  call void @llvm.dbg.value(metadata i64 1024, metadata !23, metadata !DIExpression()), !dbg !35
  %7 = tail call i8* @malloc(i64 1024), !dbg !41
  call void @llvm.dbg.value(metadata i8* %7, metadata !27, metadata !DIExpression()), !dbg !42
  store i8* %7, i8** %3, align 8, !dbg !42, !tbaa !43
  call void @llvm.dbg.value(metadata %0* %1, metadata !14, metadata !DIExpression(DW_OP_deref)), !dbg !33
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str, i64 0, i64 0), %0* nonnull %1, i8* %7), !dbg !45
  %9 = getelementptr inbounds %0, %0* %1, i64 0, i32 1
  %10 = getelementptr inbounds %0, %0* %1, i64 0, i32 0
  %11 = getelementptr inbounds %0, %0* %1, i64 0, i32 2
  br label %12, !dbg !46

; <label>:12:                                     ; preds = %22, %0
  call void @llvm.dbg.value(metadata i8* inttoptr (i64 -1 to i8*), metadata !29, metadata !DIExpression()), !dbg !47
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i64 0, i64 0)), !dbg !48
  %14 = load %struct._IO_FILE*, %struct._IO_FILE** @stdin, align 8, !dbg !49, !tbaa !43
  call void @llvm.dbg.value(metadata i64* %2, metadata !23, metadata !DIExpression(DW_OP_deref)), !dbg !35
  call void @llvm.dbg.value(metadata i8** %3, metadata !27, metadata !DIExpression(DW_OP_deref)), !dbg !42
  %15 = call i64 @getline(i8** nonnull %3, i64* nonnull %2, %struct._IO_FILE* %14) #5, !dbg !50
  call void @llvm.dbg.value(metadata i64 %15, metadata !31, metadata !DIExpression()), !dbg !51
  %16 = load i8*, i8** %3, align 8, !dbg !52, !tbaa !43
  call void @llvm.dbg.value(metadata i8* %16, metadata !27, metadata !DIExpression()), !dbg !42
  %17 = add i64 %15, -1, !dbg !53
  %18 = getelementptr inbounds i8, i8* %16, i64 %17, !dbg !52
  store i8 0, i8* %18, align 1, !dbg !54, !tbaa !55
  %19 = load i8*, i8** %3, align 8, !dbg !56, !tbaa !43
  call void @llvm.dbg.value(metadata i8* %19, metadata !27, metadata !DIExpression()), !dbg !42
  %20 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.2, i64 0, i64 0), i8* %19), !dbg !57
  %21 = load i8*, i8** %3, align 8, !dbg !58, !tbaa !43
  call void @llvm.dbg.value(metadata i8* %21, metadata !27, metadata !DIExpression()), !dbg !42
  call void @__nlex_feed(i8* %21) #5, !dbg !59
  br label %22, !dbg !60

; <label>:22:                                     ; preds = %22, %12
  %23 = phi i8* [ inttoptr (i64 -1 to i8*), %12 ], [ %28, %22 ], !dbg !61
  call void @llvm.dbg.value(metadata i8* %23, metadata !29, metadata !DIExpression()), !dbg !47
  call void @llvm.dbg.value(metadata %0* %1, metadata !14, metadata !DIExpression(DW_OP_deref)), !dbg !33
  call void @__nlex_root(%0* nonnull %1) #5, !dbg !62
  %24 = load i32, i32* %9, align 8, !dbg !64, !tbaa !65
  %25 = load i8*, i8** %10, align 8, !dbg !68, !tbaa !69
  %26 = load i8*, i8** %11, align 8, !dbg !70, !tbaa !71
  %27 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.3, i64 0, i64 0), i32 %24, i8* %25, i32 %24, i8* %26), !dbg !72
  %28 = load i8*, i8** %10, align 8, !dbg !73, !tbaa !69
  %29 = icmp eq i8* %23, %28, !dbg !75
  br i1 %29, label %12, label %22, !dbg !76, !llvm.loop !77
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
!9 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 11, type: !10, scopeLine: 11, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !0, retainedNodes: !13)
!10 = !DISubroutineType(types: !11)
!11 = !{!12}
!12 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!13 = !{!14, !23, !27, !29, !31}
!14 = !DILocalVariable(name: "res", scope: !9, file: !1, line: 12, type: !15)
!15 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "sresult", file: !1, line: 3, size: 192, elements: !16)
!16 = !{!17, !21, !22}
!17 = !DIDerivedType(tag: DW_TAG_member, name: "start", scope: !15, file: !1, line: 4, baseType: !18, size: 64)
!18 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !19, size: 64)
!19 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !20)
!20 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!21 = !DIDerivedType(tag: DW_TAG_member, name: "length", scope: !15, file: !1, line: 5, baseType: !12, size: 32, offset: 64)
!22 = !DIDerivedType(tag: DW_TAG_member, name: "tag", scope: !15, file: !1, line: 6, baseType: !18, size: 64, offset: 128)
!23 = !DILocalVariable(name: "size", scope: !9, file: !1, line: 13, type: !24)
!24 = !DIDerivedType(tag: DW_TAG_typedef, name: "size_t", file: !25, line: 62, baseType: !26)
!25 = !DIFile(filename: "/usr/lib/clang/8.0.1/include/stddef.h", directory: "")
!26 = !DIBasicType(name: "long unsigned int", size: 64, encoding: DW_ATE_unsigned)
!27 = !DILocalVariable(name: "s", scope: !9, file: !1, line: 14, type: !28)
!28 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !20, size: 64)
!29 = !DILocalVariable(name: "last", scope: !30, file: !1, line: 17, type: !28)
!30 = distinct !DILexicalBlock(scope: !9, file: !1, line: 16, column: 14)
!31 = !DILocalVariable(name: "els", scope: !30, file: !1, line: 19, type: !24)
!32 = !DILocation(line: 12, column: 5, scope: !9)
!33 = !DILocation(line: 12, column: 20, scope: !9)
!34 = !DILocation(line: 13, column: 5, scope: !9)
!35 = !DILocation(line: 13, column: 12, scope: !9)
!36 = !{!37, !37, i64 0}
!37 = !{!"long", !38, i64 0}
!38 = !{!"omnipotent char", !39, i64 0}
!39 = !{!"Simple C/C++ TBAA"}
!40 = !DILocation(line: 14, column: 5, scope: !9)
!41 = !DILocation(line: 14, column: 15, scope: !9)
!42 = !DILocation(line: 14, column: 11, scope: !9)
!43 = !{!44, !44, i64 0}
!44 = !{!"any pointer", !38, i64 0}
!45 = !DILocation(line: 15, column: 5, scope: !9)
!46 = !DILocation(line: 16, column: 5, scope: !9)
!47 = !DILocation(line: 17, column: 13, scope: !30)
!48 = !DILocation(line: 18, column: 7, scope: !30)
!49 = !DILocation(line: 19, column: 39, scope: !30)
!50 = !DILocation(line: 19, column: 20, scope: !30)
!51 = !DILocation(line: 19, column: 14, scope: !30)
!52 = !DILocation(line: 20, column: 7, scope: !30)
!53 = !DILocation(line: 20, column: 12, scope: !30)
!54 = !DILocation(line: 20, column: 16, scope: !30)
!55 = !{!38, !38, i64 0}
!56 = !DILocation(line: 21, column: 37, scope: !30)
!57 = !DILocation(line: 21, column: 7, scope: !30)
!58 = !DILocation(line: 22, column: 19, scope: !30)
!59 = !DILocation(line: 22, column: 7, scope: !30)
!60 = !DILocation(line: 23, column: 7, scope: !30)
!61 = !DILocation(line: 0, scope: !30)
!62 = !DILocation(line: 24, column: 9, scope: !63)
!63 = distinct !DILexicalBlock(scope: !30, file: !1, line: 23, column: 17)
!64 = !DILocation(line: 25, column: 38, scope: !63)
!65 = !{!66, !67, i64 8}
!66 = !{!"sresult", !44, i64 0, !67, i64 8, !44, i64 16}
!67 = !{!"int", !38, i64 0}
!68 = !DILocation(line: 25, column: 50, scope: !63)
!69 = !{!66, !44, i64 0}
!70 = !DILocation(line: 25, column: 73, scope: !63)
!71 = !{!66, !44, i64 16}
!72 = !DILocation(line: 25, column: 9, scope: !63)
!73 = !DILocation(line: 26, column: 25, scope: !74)
!74 = distinct !DILexicalBlock(scope: !63, file: !1, line: 26, column: 13)
!75 = !DILocation(line: 26, column: 18, scope: !74)
!76 = !DILocation(line: 26, column: 13, scope: !63)
!77 = distinct !{!77, !46, !78}
!78 = !DILocation(line: 30, column: 5, scope: !9)
