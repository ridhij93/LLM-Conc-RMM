; ModuleID = 'LB-simp.cpp'
source_filename = "LB-simp.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"class.std::ios_base::Init" = type { i8 }
%"struct.std::__atomic_base" = type { i32 }
%union.pthread_attr_t = type { i64, [48 x i8] }

$_ZNKSt13__atomic_baseIiEcviEv = comdat any

$_ZNSt13__atomic_baseIiEaSEi = comdat any

$_ZStanSt12memory_orderSt23__memory_order_modifier = comdat any

$__clang_call_terminate = comdat any

@_ZStL8__ioinit = internal global %"class.std::ios_base::Init" zeroinitializer, align 1
@__dso_handle = external hidden global i8
@x = dso_local global { i32 } zeroinitializer, align 4
@y = dso_local global { i32 } zeroinitializer, align 4
@value1 = dso_local global i32 0, align 4
@value2 = dso_local global i32 0, align 4
@.str = private unnamed_addr constant [27 x i8] c"value2 == 0 || value1 == 0\00", align 1
@.str.1 = private unnamed_addr constant [12 x i8] c"LB-simp.cpp\00", align 1
@__PRETTY_FUNCTION__.main = private unnamed_addr constant [11 x i8] c"int main()\00", align 1
@llvm.global_ctors = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 65535, void ()* @_GLOBAL__sub_I_LB_simp.cpp, i8* null }]

; Function Attrs: noinline uwtable
define internal void @__cxx_global_var_init() #0 section ".text.startup" {
entry:
  call void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"* @_ZStL8__ioinit)
  %0 = call i32 @__cxa_atexit(void (i8*)* bitcast (void (%"class.std::ios_base::Init"*)* @_ZNSt8ios_base4InitD1Ev to void (i8*)*), i8* getelementptr inbounds (%"class.std::ios_base::Init", %"class.std::ios_base::Init"* @_ZStL8__ioinit, i32 0, i32 0), i8* @__dso_handle) #3
  ret void
}

declare dso_local void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"*) unnamed_addr #1

; Function Attrs: nounwind
declare dso_local void @_ZNSt8ios_base4InitD1Ev(%"class.std::ios_base::Init"*) unnamed_addr #2

; Function Attrs: nounwind
declare dso_local i32 @__cxa_atexit(void (i8*)*, i8*, i8*) #3

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i8* @_Z7Thread0Pv(i8* %threadid) #4 {
entry:
  %threadid.addr = alloca i8*, align 8
  store i8* %threadid, i8** %threadid.addr, align 8
  %call = call i32 @_ZNKSt13__atomic_baseIiEcviEv(%"struct.std::__atomic_base"* bitcast ({ i32 }* @x to %"struct.std::__atomic_base"*)) #3
  store i32 %call, i32* @value1, align 4
  %call1 = call i32 @_ZNSt13__atomic_baseIiEaSEi(%"struct.std::__atomic_base"* bitcast ({ i32 }* @y to %"struct.std::__atomic_base"*), i32 1) #3
  call void @llvm.trap()
  unreachable
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local i32 @_ZNKSt13__atomic_baseIiEcviEv(%"struct.std::__atomic_base"* %this) #4 comdat align 2 personality i32 (...)* @__gxx_personality_v0 {
entry:
  %this.addr.i = alloca %"struct.std::__atomic_base"*, align 8
  %__m.addr.i = alloca i32, align 4
  %__b.i = alloca i32, align 4
  %atomic-temp.i = alloca i32, align 4
  %this.addr = alloca %"struct.std::__atomic_base"*, align 8
  store %"struct.std::__atomic_base"* %this, %"struct.std::__atomic_base"** %this.addr, align 8
  %this1 = load %"struct.std::__atomic_base"*, %"struct.std::__atomic_base"** %this.addr, align 8
  store %"struct.std::__atomic_base"* %this1, %"struct.std::__atomic_base"** %this.addr.i, align 8
  store i32 5, i32* %__m.addr.i, align 4
  %this1.i = load %"struct.std::__atomic_base"*, %"struct.std::__atomic_base"** %this.addr.i, align 8
  %0 = load i32, i32* %__m.addr.i, align 4
  %call.i = invoke i32 @_ZStanSt12memory_orderSt23__memory_order_modifier(i32 %0, i32 65535)
          to label %invoke.cont.i unwind label %terminate.lpad.i

invoke.cont.i:                                    ; preds = %entry
  store i32 %call.i, i32* %__b.i, align 4
  %_M_i.i = getelementptr inbounds %"struct.std::__atomic_base", %"struct.std::__atomic_base"* %this1.i, i32 0, i32 0
  %1 = load i32, i32* %__m.addr.i, align 4
  switch i32 %1, label %monotonic.i [
    i32 1, label %acquire.i
    i32 2, label %acquire.i
    i32 5, label %seqcst.i
  ]

monotonic.i:                                      ; preds = %invoke.cont.i
  %2 = load atomic i32, i32* %_M_i.i monotonic, align 4
  store i32 %2, i32* %atomic-temp.i, align 4
  br label %_ZNKSt13__atomic_baseIiE4loadESt12memory_order.exit

acquire.i:                                        ; preds = %invoke.cont.i, %invoke.cont.i
  %3 = load atomic i32, i32* %_M_i.i acquire, align 4
  store i32 %3, i32* %atomic-temp.i, align 4
  br label %_ZNKSt13__atomic_baseIiE4loadESt12memory_order.exit

seqcst.i:                                         ; preds = %invoke.cont.i
  %4 = load atomic i32, i32* %_M_i.i seq_cst, align 4
  store i32 %4, i32* %atomic-temp.i, align 4
  br label %_ZNKSt13__atomic_baseIiE4loadESt12memory_order.exit

terminate.lpad.i:                                 ; preds = %entry
  %5 = landingpad { i8*, i32 }
          catch i8* null
  %6 = extractvalue { i8*, i32 } %5, 0
  call void @__clang_call_terminate(i8* %6) #9
  unreachable

_ZNKSt13__atomic_baseIiE4loadESt12memory_order.exit: ; preds = %monotonic.i, %acquire.i, %seqcst.i
  %7 = load i32, i32* %atomic-temp.i, align 4
  ret i32 %7
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local i32 @_ZNSt13__atomic_baseIiEaSEi(%"struct.std::__atomic_base"* %this, i32 %__i) #4 comdat align 2 personality i32 (...)* @__gxx_personality_v0 {
entry:
  %this.addr.i = alloca %"struct.std::__atomic_base"*, align 8
  %__i.addr.i = alloca i32, align 4
  %__m.addr.i = alloca i32, align 4
  %__b.i = alloca i32, align 4
  %.atomictmp.i = alloca i32, align 4
  %this.addr = alloca %"struct.std::__atomic_base"*, align 8
  %__i.addr = alloca i32, align 4
  store %"struct.std::__atomic_base"* %this, %"struct.std::__atomic_base"** %this.addr, align 8
  store i32 %__i, i32* %__i.addr, align 4
  %this1 = load %"struct.std::__atomic_base"*, %"struct.std::__atomic_base"** %this.addr, align 8
  %0 = load i32, i32* %__i.addr, align 4
  store %"struct.std::__atomic_base"* %this1, %"struct.std::__atomic_base"** %this.addr.i, align 8
  store i32 %0, i32* %__i.addr.i, align 4
  store i32 5, i32* %__m.addr.i, align 4
  %this1.i = load %"struct.std::__atomic_base"*, %"struct.std::__atomic_base"** %this.addr.i, align 8
  %1 = load i32, i32* %__m.addr.i, align 4
  %call.i = invoke i32 @_ZStanSt12memory_orderSt23__memory_order_modifier(i32 %1, i32 65535)
          to label %invoke.cont.i unwind label %terminate.lpad.i

invoke.cont.i:                                    ; preds = %entry
  store i32 %call.i, i32* %__b.i, align 4
  %_M_i.i = getelementptr inbounds %"struct.std::__atomic_base", %"struct.std::__atomic_base"* %this1.i, i32 0, i32 0
  %2 = load i32, i32* %__m.addr.i, align 4
  %3 = load i32, i32* %__i.addr.i, align 4
  store i32 %3, i32* %.atomictmp.i, align 4
  switch i32 %2, label %monotonic.i [
    i32 3, label %release.i
    i32 5, label %seqcst.i
  ]

monotonic.i:                                      ; preds = %invoke.cont.i
  %4 = load i32, i32* %.atomictmp.i, align 4
  store atomic i32 %4, i32* %_M_i.i monotonic, align 4
  br label %_ZNSt13__atomic_baseIiE5storeEiSt12memory_order.exit

release.i:                                        ; preds = %invoke.cont.i
  %5 = load i32, i32* %.atomictmp.i, align 4
  store atomic i32 %5, i32* %_M_i.i release, align 4
  br label %_ZNSt13__atomic_baseIiE5storeEiSt12memory_order.exit

seqcst.i:                                         ; preds = %invoke.cont.i
  %6 = load i32, i32* %.atomictmp.i, align 4
  store atomic i32 %6, i32* %_M_i.i seq_cst, align 4
  br label %_ZNSt13__atomic_baseIiE5storeEiSt12memory_order.exit

terminate.lpad.i:                                 ; preds = %entry
  %7 = landingpad { i8*, i32 }
          catch i8* null
  %8 = extractvalue { i8*, i32 } %7, 0
  call void @__clang_call_terminate(i8* %8) #9
  unreachable

_ZNSt13__atomic_baseIiE5storeEiSt12memory_order.exit: ; preds = %monotonic.i, %release.i, %seqcst.i
  %9 = load i32, i32* %__i.addr, align 4
  ret i32 %9
}

; Function Attrs: cold noreturn nounwind
declare void @llvm.trap() #5

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i8* @_Z7Thread1Pv(i8* %threadid) #4 {
entry:
  %threadid.addr = alloca i8*, align 8
  store i8* %threadid, i8** %threadid.addr, align 8
  %call = call i32 @_ZNKSt13__atomic_baseIiEcviEv(%"struct.std::__atomic_base"* bitcast ({ i32 }* @y to %"struct.std::__atomic_base"*)) #3
  store i32 %call, i32* @value2, align 4
  %call1 = call i32 @_ZNSt13__atomic_baseIiEaSEi(%"struct.std::__atomic_base"* bitcast ({ i32 }* @x to %"struct.std::__atomic_base"*), i32 1) #3
  call void @llvm.trap()
  unreachable
}

; Function Attrs: noinline norecurse optnone uwtable
define dso_local i32 @main() #6 {
entry:
  %retval = alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %rc1 = alloca i32, align 4
  %rc2 = alloca i32, align 4
  %threads = alloca [2 x i64], align 16
  store i32 0, i32* %retval, align 4
  store i32 0, i32* %i, align 4
  store i32 1, i32* %j, align 4
  %arrayidx = getelementptr inbounds [2 x i64], [2 x i64]* %threads, i64 0, i64 0
  %0 = load i32, i32* %i, align 4
  %conv = sext i32 %0 to i64
  %1 = inttoptr i64 %conv to i8*
  %call = call i32 @pthread_create(i64* %arrayidx, %union.pthread_attr_t* null, i8* (i8*)* @_Z7Thread0Pv, i8* %1) #3
  store i32 %call, i32* %rc1, align 4
  %arrayidx1 = getelementptr inbounds [2 x i64], [2 x i64]* %threads, i64 0, i64 1
  %2 = load i32, i32* %j, align 4
  %conv2 = sext i32 %2 to i64
  %3 = inttoptr i64 %conv2 to i8*
  %call3 = call i32 @pthread_create(i64* %arrayidx1, %union.pthread_attr_t* null, i8* (i8*)* @_Z7Thread1Pv, i8* %3) #3
  store i32 %call3, i32* %rc2, align 4
  %arrayidx4 = getelementptr inbounds [2 x i64], [2 x i64]* %threads, i64 0, i64 0
  %4 = load i64, i64* %arrayidx4, align 16
  %call5 = call i32 @pthread_join(i64 %4, i8** null)
  %arrayidx6 = getelementptr inbounds [2 x i64], [2 x i64]* %threads, i64 0, i64 1
  %5 = load i64, i64* %arrayidx6, align 8
  %call7 = call i32 @pthread_join(i64 %5, i8** null)
  %6 = load i32, i32* @value2, align 4
  %cmp = icmp eq i32 %6, 0
  br i1 %cmp, label %lor.end, label %lor.rhs

lor.rhs:                                          ; preds = %entry
  %7 = load i32, i32* @value1, align 4
  %cmp8 = icmp eq i32 %7, 0
  br label %lor.end

lor.end:                                          ; preds = %lor.rhs, %entry
  %8 = phi i1 [ true, %entry ], [ %cmp8, %lor.rhs ]
  br i1 %8, label %cond.true, label %cond.false

cond.true:                                        ; preds = %lor.end
  br label %cond.end

cond.false:                                       ; preds = %lor.end
  call void @__assert_fail(i8* getelementptr inbounds ([27 x i8], [27 x i8]* @.str, i64 0, i64 0), i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.1, i64 0, i64 0), i32 32, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @__PRETTY_FUNCTION__.main, i64 0, i64 0)) #9
  unreachable

9:                                                ; No predecessors!
  br label %cond.end

cond.end:                                         ; preds = %9, %cond.true
  ret i32 0
}

; Function Attrs: nounwind
declare dso_local i32 @pthread_create(i64*, %union.pthread_attr_t*, i8* (i8*)*, i8*) #2

declare dso_local i32 @pthread_join(i64, i8**) #1

; Function Attrs: noreturn nounwind
declare dso_local void @__assert_fail(i8*, i8*, i32, i8*) #7

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local i32 @_ZStanSt12memory_orderSt23__memory_order_modifier(i32 %__m, i32 %__mod) #4 comdat {
entry:
  %__m.addr = alloca i32, align 4
  %__mod.addr = alloca i32, align 4
  store i32 %__m, i32* %__m.addr, align 4
  store i32 %__mod, i32* %__mod.addr, align 4
  %0 = load i32, i32* %__m.addr, align 4
  %1 = load i32, i32* %__mod.addr, align 4
  %and = and i32 %0, %1
  ret i32 %and
}

declare dso_local i32 @__gxx_personality_v0(...)

; Function Attrs: noinline noreturn nounwind
define linkonce_odr hidden void @__clang_call_terminate(i8* %0) #8 comdat {
  %2 = call i8* @__cxa_begin_catch(i8* %0) #3
  call void @_ZSt9terminatev() #9
  unreachable
}

declare dso_local i8* @__cxa_begin_catch(i8*)

declare dso_local void @_ZSt9terminatev()

; Function Attrs: noinline uwtable
define internal void @_GLOBAL__sub_I_LB_simp.cpp() #0 section ".text.startup" {
entry:
  call void @__cxx_global_var_init()
  ret void
}

attributes #0 = { noinline uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }
attributes #4 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { cold noreturn nounwind }
attributes #6 = { noinline norecurse optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #7 = { noreturn nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #8 = { noinline noreturn nounwind }
attributes #9 = { noreturn nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 11.1.0"}
