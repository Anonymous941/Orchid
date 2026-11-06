// SPDX-License-Identifier: GPL-3.0-or-later
#include <array>
#include <bit>
#include <unordered_map>
#include <catch2/catch_test_macros.hpp>
#include "shader_recompiler/environment.h"
#include "shader_recompiler/frontend/ir/basic_block.h"
#include "shader_recompiler/frontend/maxwell/translate/impl/impl.h"

namespace {
using namespace Shader;
class ShiftEnvironment final : public Environment {
public:
    u64 ReadInstruction(u32) override {
        return 0;
    }
    u32 ReadCbufValue(u32, u32) override {
        return 0;
    }
    TextureType ReadTextureType(u32) override {
        return TextureType::Color2D;
    }
    TexturePixelFormat ReadTexturePixelFormat(u32) override {
        return TexturePixelFormat::A8B8G8R8_UNORM;
    }
    bool IsTexturePixelFormatInteger(u32) override {
        return false;
    }
    u32 ReadViewportTransformState() override {
        return 0;
    }
    u32 TextureBoundBuffer() const override {
        return 0;
    }
    u32 LocalMemorySize() const override {
        return 0;
    }
    u32 SharedMemorySize() const override {
        return 0;
    }
    std::array<u32, 3> WorkgroupSize() const override {
        return {1, 1, 1};
    }
    bool HasHLEMacroState() const override {
        return false;
    }
    std::optional<ReplaceConstant> GetReplaceConstBuffer(u32, u32) override {
        return {};
    }
    void Dump(u64, u64) override {}
};

u32 RunIR(const IR::Block& block, std::array<u32, 256>& regs, bool& shift_in_range) {
    std::unordered_map<const IR::Inst*, u32> values;
    auto arg = [&](IR::Value v) -> u32 {
        if (v.IsImmediate())
            return v.Type() == IR::Type::U1 ? u32{v.U1()} : v.U32();
        return values.at(v.Inst());
    };
    for (const IR::Inst& inst : block) {
        using O = IR::Opcode;
        const auto op = inst.GetOpcode();
        u32 result{};
        switch (op) {
        case O::GetRegister:
            result = regs[static_cast<size_t>(inst.Arg(0).Reg())];
            break;
        case O::SetRegister:
            regs[static_cast<size_t>(inst.Arg(0).Reg())] = arg(inst.Arg(1));
            continue;
        case O::BitwiseAnd32:
            result = arg(inst.Arg(0)) & arg(inst.Arg(1));
            break;
        case O::ULessThan:
            result = arg(inst.Arg(0)) < arg(inst.Arg(1));
            break;
        case O::SLessThan:
            result = static_cast<s32>(arg(inst.Arg(0))) < static_cast<s32>(arg(inst.Arg(1)));
            break;
        case O::SelectU32:
            result = arg(inst.Arg(0)) ? arg(inst.Arg(1)) : arg(inst.Arg(2));
            break;
        case O::ShiftRightLogical32: {
            const u32 count = arg(inst.Arg(1));
            shift_in_range &= count < 32;
            result = count < 32 ? arg(inst.Arg(0)) >> count : 0;
            break;
        }
        case O::ShiftRightArithmetic32: {
            const u32 count = arg(inst.Arg(1));
            shift_in_range &= count < 32;
            result = count < 32 ? static_cast<u32>(static_cast<s32>(arg(inst.Arg(0))) >> count) : 0;
            break;
        }
        default:
            FAIL("Unexpected production IR opcode " << static_cast<int>(op));
        }
        values.emplace(&inst, result);
    }
    return 0;
}

TEST_CASE("Maxwell SHR translator emits defined clamped IR", "[video_core][maxwell_shr]") {
    ShiftEnvironment env;
    for (const bool sign : {false, true})
        for (const bool wrap : {false, true})
            for (const u32 dest : {20U, 21U, 22U})
                for (const u32 source : {0U, 1U, 0x80000001U, 0xffffffffU})
                    for (const u32 count : {0U, 1U, 31U, 32U, 63U, 255U}) {
                        ObjectPool<IR::Inst> pool;
                        IR::Block block{pool};
                        Maxwell::TranslatorVisitor visitor{env, block};
                        const u64 insn = 0x5c28000000000000ULL | (7ULL << 16) | dest |
                                         (21ULL << 8) | (22ULL << 20) | (u64{wrap} << 39) |
                                         (u64{sign} << 48);
                        visitor.SHR_reg(insn);
                        std::array<u32, 256> regs{};
                        regs[21] = source;
                        regs[22] = count;
                        bool in_range = true;
                        RunIR(block, regs, in_range);
                        CHECK(in_range);
                        const u32 amount = wrap ? count & 31 : std::min(count, 32U);
                        const u32 expected =
                            amount == 32 ? (sign && (source & 0x80000000U) ? ~u32{0} : 0)
                            : sign       ? static_cast<u32>(static_cast<s32>(source) >> amount)
                                         : source >> amount;
                        CHECK(regs[dest] == expected);
                    }
}
} // namespace
