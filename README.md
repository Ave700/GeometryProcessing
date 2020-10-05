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
