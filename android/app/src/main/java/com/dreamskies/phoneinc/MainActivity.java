package com.dreamskies.phoneinc;

import android.os.Bundle;
import com.dreamskies.sky.SkyActivity;

public class MainActivity extends SkyActivity {
    static {
        System.loadLibrary("phoneinc");
    }
    @Override
    public void onCreate(Bundle savedInstance) {
        super.onCreate(savedInstance);

        // nothing
    }
}
