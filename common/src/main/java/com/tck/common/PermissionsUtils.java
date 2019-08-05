package com.tck.common;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;

import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;

/**
 * <p>description:</p>
 * <p>created on: 2019/8/5 15:23</p>
 *
 * @author tck
 * @version 1.0
 */
public class PermissionsUtils {


    public static void applyStoragePermissions(@NonNull Activity context) {


    }


    public static boolean hasStoragePermissions(@NonNull Context context) {
        return ActivityCompat.checkSelfPermission(context, Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED
                && ActivityCompat.checkSelfPermission(context, Manifest.permission.READ_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED;
    }

}
