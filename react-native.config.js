module.exports = {
  dependency: {
    platforms: {
      android: {
        sourceDir: "./android",
        packageImportPath: "import com.viromedia.bridge.ReactViroPackage;",
        packageInstance:
          "new ReactViroPackage(ReactViroPackage.ViroPlatform.AR)",
      },
    },
  },
};
