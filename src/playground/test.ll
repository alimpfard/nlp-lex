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
define double @putchard(double) #0 !dbg !20 {
  %2 = alloca double, align 8
  %3 = alloca i8, align 1
  %4 = alloca i32, align 4
  store double %0, double* %2, align 8
  call void @llvm.dbg.declare(metadata double* %2, metadata !23, metadata !DIExpression()), !dbg !24
  call void @llvm.dbg.declare(metadata i8* %3, metadata !25, metadata !DIExpression()), !dbg !26
  %5 = load double, double* %2, align 8, !dbg !27
  %6 = fptosi double %5 to i8, !dbg !28
  store i8 %6, i8* %3, align 1, !dbg !26
  call void @llvm.dbg.declare(metadata i32* %4, metadata !29, metadata !DIExpression()), !dbg !30
  %7 = load i8, i8* %3, align 1, !dbg !31
  %8 = sext i8 %7 to i32, !dbg !31
  %9 = call i32 @putchar(i32 %8), !dbg !32
  store i32 %9, i32* %4, align 4, !dbg !30
  %10 = load i32, i32* %4, align 4, !dbg !33
  %11 = sitofp i32 %10 to double, !dbg !34
  ret double %11, !dbg !35
}

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

declare i32 @putchar(i32) #2

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define double @mallocd(double) #0 !dbg !36 {
  %2 = alloca double, align 8
  %3 = alloca i64, align 8
  %4 = alloca i8*, align 8
  %5 = alloca i64, align 8
  store double %0, double* %2, align 8
  call void @llvm.dbg.declare(metadata double* %2, metadata !37, metadata !DIExpression()), !dbg !38
  call void @llvm.dbg.declare(metadata i64* %3, metadata !39, metadata !DIExpression()), !dbg !40
  %6 = load double, double* %2, align 8, !dbg !41
  %7 = fptoui double %6 to i64, !dbg !42
  store i64 %7, i64* %3, align 8, !dbg !40
  call void @llvm.dbg.declare(metadata i8** %4, metadata !43, metadata !DIExpression()), !dbg !44
  %8 = load i64, i64* %3, align 8, !dbg !45
  %9 = call i8* @malloc(i64 %8), !dbg !46
  store i8* %9, i8** %4, align 8, !dbg !44
  call void @llvm.dbg.declare(metadata i64* %5, metadata !47, metadata !DIExpression()), !dbg !48
  %10 = load i8*, i8** %4, align 8, !dbg !49
  %11 = ptrtoint i8* %10 to i64, !dbg !50
  store i64 %11, i64* %5, align 8, !dbg !48
  %12 = load i64, i64* %5, align 8, !dbg !51
  %13 = sitofp i64 %12 to double, !dbg !52
  ret double %13, !dbg !53
}

declare i8* @malloc(i64) #2

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define double @memsetd(double, double, double) #0 !dbg !54 {
  %4 = alloca double, align 8
  %5 = alloca double, align 8
  %6 = alloca double, align 8
  %7 = alloca i64, align 8
  store double %0, double* %4, align 8
  call void @llvm.dbg.declare(metadata double* %4, metadata !57, metadata !DIExpression()), !dbg !58
  store double %1, double* %5, align 8
  call void @llvm.dbg.declare(metadata double* %5, metadata !59, metadata !DIExpression()), !dbg !60
  store double %2, double* %6, align 8
  call void @llvm.dbg.declare(metadata double* %6, metadata !61, metadata !DIExpression()), !dbg !62
  call void @llvm.dbg.declare(metadata i64* %7, metadata !63, metadata !DIExpression()), !dbg !64
  %8 = load double, double* %4, align 8, !dbg !65
  %9 = fptosi double %8 to i64, !dbg !65
  store i64 %9, i64* %7, align 8, !dbg !64
  %10 = load i64, i64* %7, align 8, !dbg !66
  %11 = inttoptr i64 %10 to i8*, !dbg !67
  %12 = load double, double* %5, align 8, !dbg !68
  %13 = fptosi double %12 to i32, !dbg !69
  %14 = trunc i32 %13 to i8, !dbg !70
  %15 = load double, double* %6, align 8, !dbg !71
  %16 = fptoui double %15 to i64, !dbg !72
  call void @llvm.memset.p0i8.i64(i8* align 1 %11, i8 %14, i64 %16, i1 false), !dbg !70
  %17 = ptrtoint i8* %11 to i64, !dbg !73
  store i64 %17, i64* %7, align 8, !dbg !74
  %18 = load i64, i64* %7, align 8, !dbg !75
  %19 = sitofp i64 %18 to double, !dbg !76
  ret double %19, !dbg !77
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #3

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define double @freed(double) #0 !dbg !78 {
  %2 = alloca double, align 8
  %3 = alloca i64, align 8
  store double %0, double* %2, align 8
  call void @llvm.dbg.declare(metadata double* %2, metadata !79, metadata !DIExpression()), !dbg !80
  call void @llvm.dbg.declare(metadata i64* %3, metadata !81, metadata !DIExpression()), !dbg !82
  %4 = load double, double* %2, align 8, !dbg !83
  %5 = fptosi double %4 to i64, !dbg !83
  store i64 %5, i64* %3, align 8, !dbg !82
  %6 = load i64, i64* %3, align 8, !dbg !84
  %7 = inttoptr i64 %6 to i8*, !dbg !85
  %8 = call i32 (i8*, ...) bitcast (i32 (...)* @free to i32 (i8*, ...)*)(i8* %7), !dbg !86
  %9 = sitofp i32 %8 to double, !dbg !87
  ret double %9, !dbg !88
}

declare i32 @free(...) #2

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define double @deref(double) #0 !dbg !89 {
  %2 = alloca double, align 8
  %3 = alloca i64, align 8
  store double %0, double* %2, align 8
  call void @llvm.dbg.declare(metadata double* %2, metadata !90, metadata !DIExpression()), !dbg !91
  call void @llvm.dbg.declare(metadata i64* %3, metadata !92, metadata !DIExpression()), !dbg !93
  %4 = load double, double* %2, align 8, !dbg !94
  %5 = fptosi double %4 to i64, !dbg !94
  store i64 %5, i64* %3, align 8, !dbg !93
  %6 = load i64, i64* %3, align 8, !dbg !95
  %7 = inttoptr i64 %6 to double*, !dbg !96
  %8 = load double, double* %7, align 8, !dbg !97
  ret double %8, !dbg !98
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define double @derefset(double, double) #0 !dbg !99 {
  %3 = alloca double, align 8
  %4 = alloca double, align 8
  %5 = alloca i64, align 8
  store double %0, double* %3, align 8
  call void @llvm.dbg.declare(metadata double* %3, metadata !102, metadata !DIExpression()), !dbg !103
  store double %1, double* %4, align 8
  call void @llvm.dbg.declare(metadata double* %4, metadata !104, metadata !DIExpression()), !dbg !105
  call void @llvm.dbg.declare(metadata i64* %5, metadata !106, metadata !DIExpression()), !dbg !107
  %6 = load double, double* %3, align 8, !dbg !108
  %7 = fptosi double %6 to i64, !dbg !108
  store i64 %7, i64* %5, align 8, !dbg !107
  %8 = load double, double* %4, align 8, !dbg !109
  %9 = load i64, i64* %5, align 8, !dbg !110
  %10 = inttoptr i64 %9 to double*, !dbg !111
  store double %8, double* %10, align 8, !dbg !112
  ret double %8, !dbg !113
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define i32 @main() #0 !dbg !114 {
  %1 = alloca i32, align 4
  %2 = alloca %struct.sresult, align 8
  %3 = alloca i64, align 8
  %4 = alloca i8*, align 8
  %5 = alloca i32, align 4
  %6 = alloca i64, align 8
  %7 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  call void @llvm.dbg.declare(metadata %struct.sresult* %2, metadata !117, metadata !DIExpression()), !dbg !130
  %8 = bitcast %struct.sresult* %2 to i8*, !dbg !130
  call void @llvm.memset.p0i8.i64(i8* align 8 %8, i8 0, i64 48, i1 false), !dbg !130
  call void @llvm.dbg.declare(metadata i64* %3, metadata !131, metadata !DIExpression()), !dbg !132
  store i64 1024, i64* %3, align 8, !dbg !132
  call void @llvm.dbg.declare(metadata i8** %4, metadata !133, metadata !DIExpression()), !dbg !135
  %9 = load i64, i64* %3, align 8, !dbg !136
  %10 = call i8* @malloc(i64 %9), !dbg !137
  store i8* %10, i8** %4, align 8, !dbg !135
  %11 = load i8*, i8** %4, align 8, !dbg !138
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str, i64 0, i64 0), %struct.sresult* %2, i8* %11), !dbg !139
  br label %13, !dbg !140

13:                                               ; preds = %0, %65
  call void @llvm.dbg.declare(metadata i32* %5, metadata !141, metadata !DIExpression()), !dbg !143
  store i32 -1, i32* %5, align 4, !dbg !143
  %14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.1, i64 0, i64 0)), !dbg !144
  call void @llvm.dbg.declare(metadata i64* %6, metadata !145, metadata !DIExpression()), !dbg !146
  %15 = load %struct._IO_FILE*, %struct._IO_FILE** @stdin, align 8, !dbg !147
  %16 = call i64 @getline(i8** %4, i64* %3, %struct._IO_FILE* %15), !dbg !148
  store i64 %16, i64* %6, align 8, !dbg !146
  %17 = load i8*, i8** %4, align 8, !dbg !149
  %18 = load i64, i64* %6, align 8, !dbg !150
  %19 = sub i64 %18, 1, !dbg !151
  %20 = getelementptr inbounds i8, i8* %17, i64 %19, !dbg !149
  store i8 0, i8* %20, align 1, !dbg !152
  %21 = load i8*, i8** %4, align 8, !dbg !153
  %22 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.2, i64 0, i64 0), i8* %21), !dbg !154
  %23 = load i8*, i8** %4, align 8, !dbg !155
  call void @__nlex_feed(i8* %23), !dbg !156
  br label %24, !dbg !157

24:                                               ; preds = %13, %63
  call void @__nlex_root(%struct.sresult* %2), !dbg !158
  %25 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 3, !dbg !160
  %26 = load i8, i8* %25, align 8, !dbg !160
  %27 = sext i8 %26 to i32, !dbg !161
  %28 = icmp ne i32 %27, 0, !dbg !161
  %29 = zext i1 %28 to i64, !dbg !161
  %30 = select i1 %28, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.4, i64 0, i64 0), i8* getelementptr inbounds ([1 x i8], [1 x i8]* @.str.5, i64 0, i64 0), !dbg !161
  %31 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 1, !dbg !162
  %32 = load i32, i32* %31, align 8, !dbg !162
  %33 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 0, !dbg !163
  %34 = load i8*, i8** %33, align 8, !dbg !163
  %35 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 5, !dbg !164
  %36 = load i8*, i8** %35, align 8, !dbg !164
  %37 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 1, !dbg !165
  %38 = load i32, i32* %37, align 8, !dbg !165
  %39 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 2, !dbg !166
  %40 = load i8*, i8** %39, align 8, !dbg !166
  %41 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 4, !dbg !167
  %42 = load i8, i8* %41, align 1, !dbg !167
  %43 = zext i8 %42 to i32, !dbg !168
  %44 = and i32 %43, 1, !dbg !169
  %45 = icmp ne i32 %44, 0, !dbg !168
  %46 = zext i1 %45 to i64, !dbg !168
  %47 = select i1 %45, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.6, i64 0, i64 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.7, i64 0, i64 0), !dbg !168
  %48 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([46 x i8], [46 x i8]* @.str.3, i64 0, i64 0), i8* %30, i32 %32, i8* %34, i8* %36, i32 %38, i8* %40, i8* %47), !dbg !170
  call void @llvm.dbg.declare(metadata i32* %7, metadata !171, metadata !DIExpression()), !dbg !172
  %49 = call i32 (...) @__nlex_distance(), !dbg !173
  store i32 %49, i32* %7, align 4, !dbg !172
  %50 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 3, !dbg !174
  %51 = load i8, i8* %50, align 8, !dbg !174
  %52 = sext i8 %51 to i32, !dbg !176
  %53 = icmp ne i32 %52, 0, !dbg !176
  br i1 %53, label %62, label %54, !dbg !177

54:                                               ; preds = %24
  %55 = getelementptr inbounds %struct.sresult, %struct.sresult* %2, i32 0, i32 1, !dbg !178
  %56 = load i32, i32* %55, align 8, !dbg !178
  %57 = icmp eq i32 %56, 0, !dbg !179
  br i1 %57, label %62, label %58, !dbg !180

58:                                               ; preds = %54
  %59 = load i32, i32* %7, align 4, !dbg !181
  %60 = load i32, i32* %5, align 4, !dbg !182
  %61 = icmp eq i32 %59, %60, !dbg !183
  br i1 %61, label %62, label %63, !dbg !184

62:                                               ; preds = %58, %54, %24
  br label %65, !dbg !185

63:                                               ; preds = %58
  %64 = load i32, i32* %7, align 4, !dbg !186
  store i32 %64, i32* %5, align 4, !dbg !187
  br label %24, !dbg !157, !llvm.loop !188

65:                                               ; preds = %62
  br label %13, !dbg !140, !llvm.loop !190
}

declare i32 @printf(i8*, ...) #2

declare i64 @getline(i8**, i64*, %struct._IO_FILE*) #2

declare void @__nlex_feed(i8*) #2

declare void @__nlex_root(%struct.sresult*) #2

declare i32 @__nlex_distance(...) #2

attributes #0 = { noinline nounwind optnone sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { argmemonly nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!15, !16, !17, !18}
!llvm.ident = !{!19}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 9.0.0 (tags/RELEASE_900/final)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, retainedTypes: !3, nameTableKind: None)
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
!14 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !5, size: 64)
!15 = !{i32 2, !"Dwarf Version", i32 4}
!16 = !{i32 2, !"Debug Info Version", i32 3}
!17 = !{i32 1, !"wchar_size", i32 4}
!18 = !{i32 7, !"PIC Level", i32 1}
!19 = !{!"clang version 9.0.0 (tags/RELEASE_900/final)"}
!20 = distinct !DISubprogram(name: "putchard", scope: !1, file: !1, line: 162, type: !21, scopeLine: 162, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!21 = !DISubroutineType(types: !22)
!22 = !{!5, !5}
!23 = !DILocalVariable(name: "d", arg: 1, scope: !20, file: !1, line: 162, type: !5)
!24 = !DILocation(line: 162, column: 24, scope: !20)
!25 = !DILocalVariable(name: "c", scope: !20, file: !1, line: 163, type: !4)
!26 = !DILocation(line: 163, column: 8, scope: !20)
!27 = !DILocation(line: 163, column: 19, scope: !20)
!28 = !DILocation(line: 163, column: 12, scope: !20)
!29 = !DILocalVariable(name: "x", scope: !20, file: !1, line: 164, type: !13)
!30 = !DILocation(line: 164, column: 7, scope: !20)
!31 = !DILocation(line: 164, column: 19, scope: !20)
!32 = !DILocation(line: 164, column: 11, scope: !20)
!33 = !DILocation(line: 165, column: 19, scope: !20)
!34 = !DILocation(line: 165, column: 10, scope: !20)
!35 = !DILocation(line: 165, column: 3, scope: !20)
!36 = distinct !DISubprogram(name: "mallocd", scope: !1, file: !1, line: 168, type: !21, scopeLine: 168, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!37 = !DILocalVariable(name: "s", arg: 1, scope: !36, file: !1, line: 168, type: !5)
!38 = !DILocation(line: 168, column: 23, scope: !36)
!39 = !DILocalVariable(name: "sze", scope: !36, file: !1, line: 169, type: !6)
!40 = !DILocation(line: 169, column: 10, scope: !36)
!41 = !DILocation(line: 169, column: 25, scope: !36)
!42 = !DILocation(line: 169, column: 16, scope: !36)
!43 = !DILocalVariable(name: "mem", scope: !36, file: !1, line: 170, type: !12)
!44 = !DILocation(line: 170, column: 9, scope: !36)
!45 = !DILocation(line: 170, column: 22, scope: !36)
!46 = !DILocation(line: 170, column: 15, scope: !36)
!47 = !DILocalVariable(name: "ip", scope: !36, file: !1, line: 171, type: !9)
!48 = !DILocation(line: 171, column: 14, scope: !36)
!49 = !DILocation(line: 171, column: 32, scope: !36)
!50 = !DILocation(line: 171, column: 19, scope: !36)
!51 = !DILocation(line: 172, column: 19, scope: !36)
!52 = !DILocation(line: 172, column: 10, scope: !36)
!53 = !DILocation(line: 172, column: 3, scope: !36)
!54 = distinct !DISubprogram(name: "memsetd", scope: !1, file: !1, line: 175, type: !55, scopeLine: 175, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!55 = !DISubroutineType(types: !56)
!56 = !{!5, !5, !5, !5}
!57 = !DILocalVariable(name: "p", arg: 1, scope: !54, file: !1, line: 175, type: !5)
!58 = !DILocation(line: 175, column: 23, scope: !54)
!59 = !DILocalVariable(name: "s", arg: 2, scope: !54, file: !1, line: 175, type: !5)
!60 = !DILocation(line: 175, column: 33, scope: !54)
!61 = !DILocalVariable(name: "n", arg: 3, scope: !54, file: !1, line: 175, type: !5)
!62 = !DILocation(line: 175, column: 43, scope: !54)
!63 = !DILocalVariable(name: "ip", scope: !54, file: !1, line: 176, type: !9)
!64 = !DILocation(line: 176, column: 14, scope: !54)
!65 = !DILocation(line: 176, column: 19, scope: !54)
!66 = !DILocation(line: 177, column: 36, scope: !54)
!67 = !DILocation(line: 177, column: 28, scope: !54)
!68 = !DILocation(line: 177, column: 46, scope: !54)
!69 = !DILocation(line: 177, column: 40, scope: !54)
!70 = !DILocation(line: 177, column: 21, scope: !54)
!71 = !DILocation(line: 177, column: 58, scope: !54)
!72 = !DILocation(line: 177, column: 49, scope: !54)
!73 = !DILocation(line: 177, column: 8, scope: !54)
!74 = !DILocation(line: 177, column: 6, scope: !54)
!75 = !DILocation(line: 178, column: 19, scope: !54)
!76 = !DILocation(line: 178, column: 10, scope: !54)
!77 = !DILocation(line: 178, column: 3, scope: !54)
!78 = distinct !DISubprogram(name: "freed", scope: !1, file: !1, line: 181, type: !21, scopeLine: 181, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!79 = !DILocalVariable(name: "ptr", arg: 1, scope: !78, file: !1, line: 181, type: !5)
!80 = !DILocation(line: 181, column: 21, scope: !78)
!81 = !DILocalVariable(name: "p", scope: !78, file: !1, line: 182, type: !9)
!82 = !DILocation(line: 182, column: 14, scope: !78)
!83 = !DILocation(line: 182, column: 18, scope: !78)
!84 = !DILocation(line: 183, column: 32, scope: !78)
!85 = !DILocation(line: 183, column: 24, scope: !78)
!86 = !DILocation(line: 183, column: 19, scope: !78)
!87 = !DILocation(line: 183, column: 10, scope: !78)
!88 = !DILocation(line: 183, column: 3, scope: !78)
!89 = distinct !DISubprogram(name: "deref", scope: !1, file: !1, line: 186, type: !21, scopeLine: 186, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!90 = !DILocalVariable(name: "ptr", arg: 1, scope: !89, file: !1, line: 186, type: !5)
!91 = !DILocation(line: 186, column: 21, scope: !89)
!92 = !DILocalVariable(name: "p", scope: !89, file: !1, line: 187, type: !9)
!93 = !DILocation(line: 187, column: 14, scope: !89)
!94 = !DILocation(line: 187, column: 18, scope: !89)
!95 = !DILocation(line: 188, column: 23, scope: !89)
!96 = !DILocation(line: 188, column: 13, scope: !89)
!97 = !DILocation(line: 188, column: 10, scope: !89)
!98 = !DILocation(line: 188, column: 3, scope: !89)
!99 = distinct !DISubprogram(name: "derefset", scope: !1, file: !1, line: 191, type: !100, scopeLine: 191, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!100 = !DISubroutineType(types: !101)
!101 = !{!5, !5, !5}
!102 = !DILocalVariable(name: "ptr", arg: 1, scope: !99, file: !1, line: 191, type: !5)
!103 = !DILocation(line: 191, column: 24, scope: !99)
!104 = !DILocalVariable(name: "value", arg: 2, scope: !99, file: !1, line: 191, type: !5)
!105 = !DILocation(line: 191, column: 36, scope: !99)
!106 = !DILocalVariable(name: "p", scope: !99, file: !1, line: 192, type: !9)
!107 = !DILocation(line: 192, column: 14, scope: !99)
!108 = !DILocation(line: 192, column: 18, scope: !99)
!109 = !DILocation(line: 193, column: 28, scope: !99)
!110 = !DILocation(line: 193, column: 23, scope: !99)
!111 = !DILocation(line: 193, column: 13, scope: !99)
!112 = !DILocation(line: 193, column: 26, scope: !99)
!113 = !DILocation(line: 193, column: 3, scope: !99)
!114 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 197, type: !115, scopeLine: 197, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!115 = !DISubroutineType(types: !116)
!116 = !{!13}
!117 = !DILocalVariable(name: "res", scope: !114, file: !1, line: 199, type: !118)
!118 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "sresult", file: !1, line: 4, size: 384, elements: !119)
!119 = !{!120, !123, !124, !125, !126, !128, !129}
!120 = !DIDerivedType(tag: DW_TAG_member, name: "start", scope: !118, file: !1, line: 5, baseType: !121, size: 64)
!121 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !122, size: 64)
!122 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !4)
!123 = !DIDerivedType(tag: DW_TAG_member, name: "length", scope: !118, file: !1, line: 6, baseType: !13, size: 32, offset: 64)
!124 = !DIDerivedType(tag: DW_TAG_member, name: "tag", scope: !118, file: !1, line: 7, baseType: !121, size: 64, offset: 128)
!125 = !DIDerivedType(tag: DW_TAG_member, name: "errc", scope: !118, file: !1, line: 8, baseType: !4, size: 8, offset: 192)
!126 = !DIDerivedType(tag: DW_TAG_member, name: "metadata", scope: !118, file: !1, line: 9, baseType: !127, size: 8, offset: 200)
!127 = !DIBasicType(name: "unsigned char", size: 8, encoding: DW_ATE_unsigned_char)
!128 = !DIDerivedType(tag: DW_TAG_member, name: "pos", scope: !118, file: !1, line: 10, baseType: !121, size: 64, offset: 256)
!129 = !DIDerivedType(tag: DW_TAG_member, name: "allocd", scope: !118, file: !1, line: 11, baseType: !13, size: 32, offset: 320)
!130 = !DILocation(line: 199, column: 18, scope: !114)
!131 = !DILocalVariable(name: "size", scope: !114, file: !1, line: 200, type: !6)
!132 = !DILocation(line: 200, column: 10, scope: !114)
!133 = !DILocalVariable(name: "s", scope: !114, file: !1, line: 201, type: !134)
!134 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !4, size: 64)
!135 = !DILocation(line: 201, column: 9, scope: !114)
!136 = !DILocation(line: 201, column: 20, scope: !114)
!137 = !DILocation(line: 201, column: 13, scope: !114)
!138 = !DILocation(line: 202, column: 40, scope: !114)
!139 = !DILocation(line: 202, column: 3, scope: !114)
!140 = !DILocation(line: 203, column: 3, scope: !114)
!141 = !DILocalVariable(name: "last", scope: !142, file: !1, line: 204, type: !13)
!142 = distinct !DILexicalBlock(scope: !114, file: !1, line: 203, column: 13)
!143 = !DILocation(line: 204, column: 9, scope: !142)
!144 = !DILocation(line: 205, column: 5, scope: !142)
!145 = !DILocalVariable(name: "els", scope: !142, file: !1, line: 206, type: !6)
!146 = !DILocation(line: 206, column: 12, scope: !142)
!147 = !DILocation(line: 206, column: 37, scope: !142)
!148 = !DILocation(line: 206, column: 18, scope: !142)
!149 = !DILocation(line: 207, column: 5, scope: !142)
!150 = !DILocation(line: 207, column: 7, scope: !142)
!151 = !DILocation(line: 207, column: 11, scope: !142)
!152 = !DILocation(line: 207, column: 16, scope: !142)
!153 = !DILocation(line: 208, column: 35, scope: !142)
!154 = !DILocation(line: 208, column: 5, scope: !142)
!155 = !DILocation(line: 210, column: 17, scope: !142)
!156 = !DILocation(line: 210, column: 5, scope: !142)
!157 = !DILocation(line: 211, column: 5, scope: !142)
!158 = !DILocation(line: 213, column: 7, scope: !159)
!159 = distinct !DILexicalBlock(scope: !142, file: !1, line: 211, column: 48)
!160 = !DILocation(line: 216, column: 19, scope: !159)
!161 = !DILocation(line: 216, column: 15, scope: !159)
!162 = !DILocation(line: 216, column: 43, scope: !159)
!163 = !DILocation(line: 216, column: 55, scope: !159)
!164 = !DILocation(line: 216, column: 66, scope: !159)
!165 = !DILocation(line: 217, column: 18, scope: !159)
!166 = !DILocation(line: 217, column: 30, scope: !159)
!167 = !DILocation(line: 217, column: 40, scope: !159)
!168 = !DILocation(line: 217, column: 36, scope: !159)
!169 = !DILocation(line: 217, column: 49, scope: !159)
!170 = !DILocation(line: 215, column: 7, scope: !159)
!171 = !DILocalVariable(name: "dist", scope: !159, file: !1, line: 218, type: !13)
!172 = !DILocation(line: 218, column: 11, scope: !159)
!173 = !DILocation(line: 218, column: 18, scope: !159)
!174 = !DILocation(line: 219, column: 15, scope: !175)
!175 = distinct !DILexicalBlock(scope: !159, file: !1, line: 219, column: 11)
!176 = !DILocation(line: 219, column: 11, scope: !175)
!177 = !DILocation(line: 219, column: 20, scope: !175)
!178 = !DILocation(line: 219, column: 27, scope: !175)
!179 = !DILocation(line: 219, column: 34, scope: !175)
!180 = !DILocation(line: 219, column: 39, scope: !175)
!181 = !DILocation(line: 219, column: 42, scope: !175)
!182 = !DILocation(line: 219, column: 50, scope: !175)
!183 = !DILocation(line: 219, column: 47, scope: !175)
!184 = !DILocation(line: 219, column: 11, scope: !159)
!185 = !DILocation(line: 220, column: 9, scope: !175)
!186 = !DILocation(line: 221, column: 14, scope: !159)
!187 = !DILocation(line: 221, column: 12, scope: !159)
!188 = distinct !{!188, !157, !189}
!189 = !DILocation(line: 222, column: 5, scope: !142)
!190 = distinct !{!190, !140, !191}
!191 = !DILocation(line: 223, column: 3, scope: !114)
