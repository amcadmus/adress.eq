#!/bin/bash

function ibi_setting () {
    sed -e "s/<iterations_max>.*<\/iterations_max>/<iterations_max>$tf_iterations_max<\/iterations_max>/g" settings.xml > settings.xml.tmp
    mv -f settings.xml.tmp settings.xml
}

function tf_setting () {
    sed -e "s/<min>.*<\/min>/<min>$tf_min<\/min>/g" settings.xml |\
    sed -e "s/<max>.*<\/max>/<max>$tf_max<\/max>/g" |\
    sed -e "s/<step>.*<\/step>/<step>$tf_step<\/step>/g" |\
    sed -e "s/<spline_start>.*<\/spline_start>/<spline_start>$tf_spline_start<\/spline_start>/g" |\
    sed -e "s/<spline_end>.*<\/spline_end>/<spline_end>$tf_spline_end<\/spline_end>/g" |\
    sed -e "s/<spline_step>.*<\/spline_step>/<spline_step>$tf_spline_step<\/spline_step>/g" |\
    sed -e "s/<table_end>.*<\/table_end>/<table_end>$half_boxx_1<\/table_end>/g" |\
    sed -e "s/<iterations_max>.*<\/iterations_max>/<iterations_max>$tf_iterations_max<\/iterations_max>/g" > settings.xml.tmp
    mv -f settings.xml.tmp settings.xml
}

