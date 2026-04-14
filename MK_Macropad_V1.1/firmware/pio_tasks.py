Import("env")

def run_step(label, command):
    print(f"\n{label}...")
    result = env.Execute(command)
    if result != 0:
        print(f"❌ ÉCHEC : {label}")
        return False
    return True

def full_build(target, source, env):

    if not run_step("🧹 CLEAN", "pio run -t clean"):
        return

    if not run_step("🔨 BUILD", "pio run"):
        return

    if not run_step("📦 BUILD FILESYSTEM", "pio run -t buildfs"):
        return

    if not run_step("⬆️ UPLOAD FIRMWARE", "pio run -t upload"):
        return

    if not run_step("⬆️ UPLOAD FILESYSTEM", "pio run -t uploadfs"):
        return

    print("\n🎉 DONE!")

env.AddCustomTarget(
    name="full",
    dependencies=None,
    actions=[full_build],
    title="Full Build + Upload + FS",
    description="Clean, build, buildfs, upload, uploadfs"
)
