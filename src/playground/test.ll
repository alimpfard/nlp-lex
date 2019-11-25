; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i64, i16, i8, [1 x i8], i8*, i64, %struct._IO_codecvt*, %struct._IO_wide_data*, %struct._IO_FILE*, i8*, i64, i32, [20 x i8] }
%struct._IO_marker = type opaque
%struct._IO_codecvt = type opaque
%struct._IO_wide_data = type opaque
%struct.sresult = type { i8*, i32, i8*, i8, i8, i8*, i32 }

@.str = private unnamed_addr constant [20 x i8] c"res at %p, s at %p\0A\00", align 1
@.str.1 = private unnamed_addr constant [3 x i8] c"> \00", align 1
@stdin = external global %struct._IO_FILE*, align 8
@.str.2 = private unnamed_addr constant [19 x i8] c"processing - '%s'\0A\00", align 1
@.str.3 = private unnamed_addr constant [46 x i8] c"%smatch {'%.*s' - %s - %d %s} is%sa stopword\0A\00", align 1
@.str.4 = private unnamed_addr constant [4 x i8] c"no \00", align 1
@.str.5 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@.str.6 = private unnamed_addr constant [2 x i8] c" \00", align 1
@.str.7 = private unnamed_addr constant [6 x i8] c" not \00", align 1

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define i32 @main() #0 !dbg !8 {
  %1 = alloca i32, align 4
  %2 = alloca %struct.sresult, align 8
  %3 = alloca i64, align 8
  %4 = alloca i8*, align 8
  %5 = alloca i32, align 4
  %6 = alloca i64, align 8
  %7 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  call void @llvm.dbg.declare(metadata %struct.sresult* %2, metadata !12, metadata !DIExpression()), !dbg !26
  %8 = bitcast %struct.sresult* %2 to i8*, !dbg !26
  call void @llvm.memset.p0i8.i64(i8* align 8 %8, i8 0, i64 48, i1 false), !dbg !26
  call void @llvm.dbg.declare(metadata i64* %3, metadata !27, metadata !DIExpression()), !dbg !31
  store i64 1024, i64* %3, align 8, !dbg !31
  call void @llvm.dbg.declare(metadata i8** %4, metadata !32, metadata !DIExpression()), !dbg !34
  %9 = load i64, i64* %3, align 8, !dbg !35
  %10 = call i8* @malloc(i64 %9), !dbg !36
  store i8* %10, i8** %4, align 8, !dbg !34
  %11 = load i8*, i8** %4, align 8, !dbg !37
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str, i64 0, i64 0), %struct.sresult* %2, i8* %11), !dbg !38
  br label %13, !dbg !39

13:                                               ; preds = %0, %65
  call void @llvm.dbg.declare(metadata i32* %5, metadata !40, metadata !DIExpression()), !dbg !42
  store i32 -1, i32* %5, align 4, !dbg !42
  %14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i64 0, i64 0)), !dbg !43
  call void @llvm.dbg.declare(metadata i64* %6, metadata !44, metadata !DIExpression()), !dbg !45
  %15 = load %struct._IO_FILE*, %struct._IO_FILE** @stdin, align 8, !dbg !46
  %16 = call i64 @getline(i8** %4, i64* %3, %struct._IO_FILE* %15), !dbg !47
  store i64 %16, i64* %6, align 8, !dbg !45
  %17 = load i8*, i8** %4, align 8, !dbg !48
  %18 = load i64, i64* %6, align 8, !dbg !49
  %19 = sub i64 %18, 1, !dbg !50
  %20 = getelementptr inbounds i8, i8* %17, i64 %19, !dbg !48
  store i8 0, i8* %20, align 1, !dbg !51
  %21 = load i8*, i8** %4, align 8, !dbg !52
  %22 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.2, i64 0, i64 0), i8* %21), !dbg !53
  %23 = load i8*, i8** %4, align 8, !dbg !54
  call void @__nlex_feed(i8* %23), !dbg !55
  br label %24, !dbg !56

24:                                               ; preds = %13, %63
  call void @__nlex_root(%struct.sresult* %2), !dbg !57
  %25 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 3, !dbg !59
  %26 = load i8, i8* %25, align 8, !dbg !59
  %27 = sext i8 %26 to i32, !dbg !60
  %28 = icmp ne i32 %27, 0, !dbg !60
  %29 = zext i1 %28 to i64, !dbg !60
  %30 = select i1 %28, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.4, i64 0, i64 0), i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str.5, i64 0, i64 0), !dbg !60
  %31 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 1, !dbg !61
  %32 = load i32, i32* %31, align 8, !dbg !61
  %33 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 0, !dbg !62
  %34 = load i8*, i8** %33, align 8, !dbg !62
  %35 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 5, !dbg !63
  %36 = load i8*, i8** %35, align 8, !dbg !63
  %37 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 1, !dbg !64
  %38 = load i32, i32* %37, align 8, !dbg !64
  %39 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 2, !dbg !65
  %40 = load i8*, i8** %39, align 8, !dbg !65
  %41 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 4, !dbg !66
  %42 = load i8, i8* %41, align 1, !dbg !66
  %43 = zext i8 %42 to i32, !dbg !67
  %44 = and i32 %43, 1, !dbg !68
  %45 = icmp ne i32 %44, 0, !dbg !67
  %46 = zext i1 %45 to i64, !dbg !67
  %47 = select i1 %45, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.6, i64 0, i64 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.7, i64 0, i64 0), !dbg !67
  %48 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([46 x i8], [46 x i8]* @.str.3, i64 0, i64 0), i8* %30, i32 %32, i8* %34, i8* %36, i32 %38, i8* %40, i8* %47), !dbg !69
  call void @llvm.dbg.declare(metadata i32* %7, metadata !70, metadata !DIExpression()), !dbg !71
  %49 = call i32 (...) @__nlex_distance(), !dbg !72
  store i32 %49, i32* %7, align 4, !dbg !71
  %50 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 3, !dbg !73
  %51 = load i8, i8* %50, align 8, !dbg !73
  %52 = sext i8 %51 to i32, !dbg !75
  %53 = icmp ne i32 %52, 0, !dbg !75
  br i1 %53, label %62, label %54, !dbg !76

54:                                               ; preds = %24
  %55 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 1, !dbg !77
  %56 = load i32, i32* %55, align 8, !dbg !77
  %57 = icmp eq i32 %56, 0, !dbg !78
  br i1 %57, label %62, label %58, !dbg !79

58:                                               ; preds = %54
  %59 = load i32, i32* %7, align 4, !dbg !80
  %60 = load i32, i32* %5, align 4, !dbg !81
  %61 = icmp eq i32 %59, %60, !dbg !82
  br i1 %61, label %62, label %63, !dbg !83

62:                                               ; preds = %58, %54, %24
  br label %65, !dbg !84

63:                                               ; preds = %58
  %64 = load i32, i32* %7, align 4, !dbg !85
  store i32 %64, i32* %5, align 4, !dbg !86
  br label %24, !dbg !56, !llvm.loop !87

65:                                               ; preds = %62
  br label %13, !dbg !39, !llvm.loop !89
}

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #2

declare i8* @malloc(i64) #3

declare i32 @printf(i8*, ...) #3

declare i64 @getline(i8**, i64*, %struct._IO_FILE*) #3

declare void @__nlex_feed(i8*) #3

declare void @__nlex_root(%struct.sresult*) #3

declare i32 @__nlex_distance(...) #3

attributes #0 = { noinline nounwind optnone sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable }
attributes #2 = { argmemonly nounwind }
attributes #3 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4, !5, !6}
!llvm.ident = !{!7}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 9.0.0 (tags/RELEASE_900/final)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, nameTableKind: None)
!1 = !DIFile(filename: "test.c", directory: "/home/Test/Documents/nlp-lex/src/playground")
!2 = !{}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{i32 7, !"PIC Level", i32 1}
!7 = !{!"clang version 9.0.0 (tags/RELEASE_900/final)"}
!8 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 161, type: !9, scopeLine: 161, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!9 = !DISubroutineType(types: !10)
!10 = !{!11}
!11 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!12 = !DILocalVariable(name: "res", scope: !8, file: !1, line: 163, type: !13)
!13 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "sresult", file: !1, line: 3, size: 384, elements: !14)
!14 = !{!15, !19, !20, !21, !22, !24, !25}
!15 = !DIDerivedType(tag: DW_TAG_member, name: "start", scope: !13, file: !1, line: 4, baseType: !16, size: 64)
!16 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !17, size: 64)
!17 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !18)
!18 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!19 = !DIDerivedType(tag: DW_TAG_member, name: "length", scope: !13, file: !1, line: 5, baseType: !11, size: 32, offset: 64)
!20 = !DIDerivedType(tag: DW_TAG_member, name: "tag", scope: !13, file: !1, line: 6, baseType: !16, size: 64, offset: 128)
!21 = !DIDerivedType(tag: DW_TAG_member, name: "errc", scope: !13, file: !1, line: 7, baseType: !18, size: 8, offset: 192)
!22 = !DIDerivedType(tag: DW_TAG_member, name: "metadata", scope: !13, file: !1, line: 8, baseType: !23, size: 8, offset: 200)
!23 = !DIBasicType(name: "unsigned char", size: 8, encoding: DW_ATE_unsigned_char)
!24 = !DIDerivedType(tag: DW_TAG_member, name: "pos", scope: !13, file: !1, line: 9, baseType: !16, size: 64, offset: 256)
!25 = !DIDerivedType(tag: DW_TAG_member, name: "allocd", scope: !13, file: !1, line: 10, baseType: !11, size: 32, offset: 320)
!26 = !DILocation(line: 163, column: 18, scope: !8)
!27 = !DILocalVariable(name: "size", scope: !8, file: !1, line: 164, type: !28)
!28 = !DIDerivedType(tag: DW_TAG_typedef, name: "size_t", file: !29, line: 46, baseType: !30)
!29 = !DIFile(filename: "/usr/lib/clang/9.0.0/include/stddef.h", directory: "")
!30 = !DIBasicType(name: "long unsigned int", size: 64, encoding: DW_ATE_unsigned)
!31 = !DILocation(line: 164, column: 10, scope: !8)
!32 = !DILocalVariable(name: "s", scope: !8, file: !1, line: 165, type: !33)
!33 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !18, size: 64)
!34 = !DILocation(line: 165, column: 9, scope: !8)
!35 = !DILocation(line: 165, column: 20, scope: !8)
!36 = !DILocation(line: 165, column: 13, scope: !8)
!37 = !DILocation(line: 166, column: 40, scope: !8)
!38 = !DILocation(line: 166, column: 3, scope: !8)
!39 = !DILocation(line: 167, column: 3, scope: !8)
!40 = !DILocalVariable(name: "last", scope: !41, file: !1, line: 168, type: !11)
!41 = distinct !DILexicalBlock(scope: !8, file: !1, line: 167, column: 13)
!42 = !DILocation(line: 168, column: 9, scope: !41)
!43 = !DILocation(line: 169, column: 5, scope: !41)
!44 = !DILocalVariable(name: "els", scope: !41, file: !1, line: 170, type: !28)
!45 = !DILocation(line: 170, column: 12, scope: !41)
!46 = !DILocation(line: 170, column: 37, scope: !41)
!47 = !DILocation(line: 170, column: 18, scope: !41)
!48 = !DILocation(line: 171, column: 5, scope: !41)
!49 = !DILocation(line: 171, column: 7, scope: !41)
!50 = !DILocation(line: 171, column: 11, scope: !41)
!51 = !DILocation(line: 171, column: 16, scope: !41)
!52 = !DILocation(line: 172, column: 35, scope: !41)
!53 = !DILocation(line: 172, column: 5, scope: !41)
!54 = !DILocation(line: 174, column: 17, scope: !41)
!55 = !DILocation(line: 174, column: 5, scope: !41)
!56 = !DILocation(line: 175, column: 5, scope: !41)
!57 = !DILocation(line: 177, column: 7, scope: !58)
!58 = distinct !DILexicalBlock(scope: !41, file: !1, line: 175, column: 48)
!59 = !DILocation(line: 180, column: 19, scope: !58)
!60 = !DILocation(line: 180, column: 15, scope: !58)
!61 = !DILocation(line: 180, column: 43, scope: !58)
!62 = !DILocation(line: 180, column: 55, scope: !58)
!63 = !DILocation(line: 180, column: 66, scope: !58)
!64 = !DILocation(line: 181, column: 18, scope: !58)
!65 = !DILocation(line: 181, column: 30, scope: !58)
!66 = !DILocation(line: 181, column: 40, scope: !58)
!67 = !DILocation(line: 181, column: 36, scope: !58)
!68 = !DILocation(line: 181, column: 49, scope: !58)
!69 = !DILocation(line: 179, column: 7, scope: !58)
!70 = !DILocalVariable(name: "dist", scope: !58, file: !1, line: 182, type: !11)
!71 = !DILocation(line: 182, column: 11, scope: !58)
!72 = !DILocation(line: 182, column: 18, scope: !58)
!73 = !DILocation(line: 183, column: 15, scope: !74)
!74 = distinct !DILexicalBlock(scope: !58, file: !1, line: 183, column: 11)
!75 = !DILocation(line: 183, column: 11, scope: !74)
!76 = !DILocation(line: 183, column: 20, scope: !74)
!77 = !DILocation(line: 183, column: 27, scope: !74)
!78 = !DILocation(line: 183, column: 34, scope: !74)
!79 = !DILocation(line: 183, column: 39, scope: !74)
!80 = !DILocation(line: 183, column: 42, scope: !74)
!81 = !DILocation(line: 183, column: 50, scope: !74)
!82 = !DILocation(line: 183, column: 47, scope: !74)
!83 = !DILocation(line: 183, column: 11, scope: !58)
!84 = !DILocation(line: 184, column: 9, scope: !74)
!85 = !DILocation(line: 185, column: 14, scope: !58)
!86 = !DILocation(line: 185, column: 12, scope: !58)
!87 = distinct !{!87, !56, !88}
!88 = !DILocation(line: 186, column: 5, scope: !41)
!89 = distinct !{!89, !39, !90}
!90 = !DILocation(line: 187, column: 3, scope: !8)
