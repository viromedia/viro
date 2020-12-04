module.exports = {
  dependency: {
    platforms: {
      android: {
        sourceDir: "./android/viro_bridge",
        packageImportPath: "import com.viromedia.bridge.ReactViroPackage;",
        packageInstance:
          "new ReactViroPackage(ReactViroPackage.ViroPlatform.AR)",
      },
    },
  },
};
