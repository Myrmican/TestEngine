// TEMPORARY DEBUG SHADER — no cbuffer, no uniform data, just a hardcoded
// bright color. This has zero dependency on uniform buffer binding, so
// it isolates one specific question: does the fragment stage's color
// output reach the screen at all, independent of any uniform push?
//
// Compile to shaders/compiled/triangle.frag.spv temporarily, test
// alongside the DEBUG_NO_MVP vertex shader (keep that one in place too
// for this test — one variable at a time), then swap both back to the
// real shaders afterward.
//
// - Magenta square appears -> color/blend/presentation pipeline is
//   fine. The bug is specifically the Color uniform buffer not
//   reaching the shader (binding/push issue) — likely worth trying
//   SDL_GPU_SHADERFORMAT and register conventions again, or checking
//   whether num_uniform_buffers passed to LoadShader() in Renderer.cpp
//   actually matches what's really compiled into the shader.
// - Still black -> something more fundamental: possibly the STUDIO
//   executable wasn't actually rebuilt after a recent C++ change (worth
//   doing a full clean rebuild, not just recompiling shaders — these
//   are separate build steps), or a color target / blend state issue.

float4 main() : SV_Target0
{
    return float4(1.0f, 0.0f, 1.0f, 1.0f); // magenta
}
