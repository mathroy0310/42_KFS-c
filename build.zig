const std = @import("std");

const version = std.SemanticVersion.parse("1") catch unreachable;

pub fn build(b: *std.Build) void {
    const exe = b.addExecutable(.{
        // Name of your program
        .name = "kfs.bin",
        .target = b.resolveTargetQuery(
			.{
				.cpu_arch = .x86,
				.os_tag = .freestanding,
				.abi = .none,
			}
		),
		.code_model = .kernel,
        .optimize = b.standardOptimizeOption(.{}),
		.link_libc = false,
    });

    exe.addIncludePath(.{ .path = "src" });

    exe.addCSourceFiles(.{
        .files = &.{
            "src/kernel/arch/i386/start.c",
        },
        .flags = &.{
			"-fno-builtin",
			"-fno-exceptions",
			"-fno-stack-protector",
			"-fno-rtti",
			"-nostdlib",
			"-nodefaultlibs",
            "-Wall",
            "-Wextra",
        },
    });
	
	exe.setLinkerScript(.{ .path = "src/kernel/linker-i386.ld"});

    b.installArtifact(exe);



}
