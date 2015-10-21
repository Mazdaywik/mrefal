#include "refalrts.h"
#include "mr_profiler.h"

refalrts::FnResult MR_Profiler_Quantify(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_profiler::quantify(arg_begin, arg_end);
}

refalrts::FnResult MR_Profiler_PrintResults(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_profiler::print_results(arg_begin, arg_end);
}

refalrts::FnResult MR_Profiler_SetOutputName(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_profiler::set_output_name(arg_begin, arg_end);
}

refalrts::FnResult MR_Profiler_Flush(
  refalrts::Iter arg_begin, refalrts::Iter arg_end
) {
  return implement_profiler::flush(arg_begin, arg_end);
}