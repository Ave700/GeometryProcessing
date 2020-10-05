# GeometryProcessing

This is a geometry processing research project I am doing with Dr. Zhang at Oregon State University.

The application is built in OpenGL and QT. 

# File Structure

<ul>
<li>3rdparty
    <ul>
    <li>GL</li>
        -libs/includes (glew, glut, etc)
    <li>assimp</li>
        -libs/inlcludes (model loading)
    <li>ImageMagick</li>
        -libs/includes (image loading for textures)
     </ul>
</li>        
<li>bin
    <ul>
    <li>Dlls for running including QT dlls</li>
    <li>Build location and executable of final program</li>
    </ul>
</li>
<li>models
    <ul>
    <li>bunny</li>
        -4 resolutions of the stanford buny
    <li>oloid</li>
        -4 resolutions of an oloid object
    <li>spot</li>
        -single file of a cow object
     </ul>
</li>        
<li>All source and header files are in the main directory</li>
</ul>

# Code Structure

<ul>
<li>Main</li>
    Contains main window call and initialization call
<li>mainwindow</li>
    Contians GlWindow call and file open gui code
<li>contourwidget</li>
    <p>Contains openGL code for contour implementation (used currently),
    this is the bulk of the applicaiton.</p>
</ul>    

# Building instructions
<p>Building applicaiton requires current version of QT creator.
I built the application using Qt 5.15. No guarentee it works with any 
other versions. If you want to use MSVC I would not reccomend it, and 
at this time won't provide any information as to how to build in it. All 
dependencies are in the 3rd party folder. Also currently this only is setup
for windows, and I have not attempted to use it with Mac OS yet.</p>
<ol>
<li>If Qt is not installed go to QT's website and download it: https://www.qt.io/download </li>
    I'd reccomend while installing only selecting the version you need ( for instance 5.15 > msvc2019 )
<li>Open the QTWidgetContour.pro file</li>
<li>Import for you development enviornment (probably only works for 32bit)
<li>At this point it should build properly</li>
</ol>


# QT Theme
<p> I've included a dark theme for QT Creator that mimics msvc 2019. To include it in QT Creator
copy the file to C:\Users\ (yourusername)\AppData\Roaming\QtProject\qtcreator\styles</p>
