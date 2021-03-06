/*
	Offscreen Android Views library for Qt

	Authors:
	Evgeniy A. Samoylov <ghelius@gmail.com>

	Distrbuted under The BSD License

	Copyright (c) 2016, DoubleGIS, LLC.
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	* Redistributions of source code must retain the above copyright notice,
	  this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright notice,
	  this list of conditions and the following disclaimer in the documentation
	  and/or other materials provided with the distribution.
	* Neither the name of the DoubleGIS, LLC nor the names of its contributors
	  may be used to endorse or promote products derived from this software
	  without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
	THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
	ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
	BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
	CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
	SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
	CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
	ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
	THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "QAndroidBatteryDataProvider.h"
#include <QAndroidQPAPluginGap.h>
#include "TJniObjectLinker.h"


namespace Mobility {

Q_DECL_EXPORT void JNICALL Java_BatteryListener_batteryInfoUpdate(JNIEnv *, jobject, jlong native_ptr, jboolean plugged, jint level)
{
	JNI_LINKER_OBJECT(Mobility::QAndroidBatteryDataProvider, native_ptr, proxy)
	proxy->batteryInfo(plugged, level);
}


static const JNINativeMethod methods[] = {
	{"getContext", "()Landroid/content/Context;", (void*)QAndroidQPAPluginGap::getCurrentContextNoThrow},
	{"batteryInfoUpdate", "(JZI)V", (void*)Java_BatteryListener_batteryInfoUpdate},
};


JNI_LINKER_IMPL(QAndroidBatteryDataProvider, "ru/dublgis/androidhelpers/mobility/BatteryListener", methods)


QAndroidBatteryDataProvider::QAndroidBatteryDataProvider(QObject * parent)
	: QObject(parent)
	, jniLinker_(new JniObjectLinker(this))
{
}


QAndroidBatteryDataProvider::~QAndroidBatteryDataProvider()
{
}


void QAndroidBatteryDataProvider::start()
{
	if (isJniReady())
	{
		jni()->callBool("start");
	}
}


void QAndroidBatteryDataProvider::stop()
{
	if (isJniReady())
	{
		jni()->callVoid("stop");
	}
}

void QAndroidBatteryDataProvider::batteryInfo(bool plugged, int level)
{
	emit batteryInfoUpdate(plugged, level);
}

}
