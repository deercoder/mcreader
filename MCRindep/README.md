README
===


##Target##

This project is to use VlFeat + OpenCV under android, to build an application that could recognize the object by taking pictures via the camera with some detection/classification algorithms.


## old-Notes(for jni-myold folder version) ##

* must edit Application.mk to add "APP_PLATFORM := android-9", otherwise it will not link successfully.

* reference:[dev with OpenCV on Andriod](http://docs.opencv.org/doc/tutorials/introduction/android_binary_package/dev_with_OCV_on_Android.html).

* also cross-refer the ndk-build tutorial.


## Notes(for jni folder)##

* This is for a newer version and build system, since the older jni contains out-dated files and libraries.

* To fix build error, check `Android.mk` for some linked libs, build flags/macros.

* If there is some undefined reference to `XXX` error information, it should be:

    * some functions are ill-defined, maybe we called some functions withou any implementation, try to compare the source code, find it and adds to our repo
    * if original VLfeat doesn't have one, then it should be some Makefile defintion, since we know that missing a tiny flag can result in a disaster when building with many strange error...
    * be calm if there are many error infos, many of the information are just caused by one or two error, find the source file that incurred the error, open the editor and check the source file
    * if the error info is caused by unknow function AND the function actually is not existed in original vlfeat, then it's not our porting problem, it's because Makefile or compile/link rules error.
    * In this case, it's cause by Macros, since original Makefile.am has flags to disable SSE and AVX, but when port in andriod, we just use Android.mk and don't add the flags, which caused the compiler cannot find the function definition.
    * Solution is simple, just add the Macros and rebuild it again with `andriod-ndk`



## Work done ##

This project requires ndk-build, opencv.mk configuration, as well vlfeat native code. so our work involvs:

1) add upper-layer java code, notes about its library configuration.

2) add native code, which is ported from vlfeat library, and also may include the opencv structure and type

3) ndk-build native code, which is written in step 2).

4) eclipse build the whole project, package the libs, note all the environment configuration.

when building the native code, please note about the ndk path in $PATH, also note about the OpenCV.mk in Android.mk, using command line to ndk-build the .so library.
