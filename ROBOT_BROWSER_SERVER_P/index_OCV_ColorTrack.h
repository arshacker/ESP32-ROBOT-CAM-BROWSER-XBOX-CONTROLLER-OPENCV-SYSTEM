/****************************
  This include file, index_OCV_ColorTrack.h, the Client, is an intoduction of OpenCV.js to the ESP32 Camera environment. The Client was
  developed and written by Andrew R. Sass. Permission to reproduce the index_OCV_ColorTrack.h file is granted free of charge if this
  entire copyright notice is included in all copies of the index_OCV_ColorTrack.h file. 
*******************************/
static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!DOCTYPE html>
<head>
   <title>ESP32-CAMERA COLOR DETECTION</title>
   <meta charset="utf-8">
   <meta name="viewport" content="width=device-width,initial-scale=1">
   <!----ANN:3--->
   <!--<script async src="http://192.168.4.2:8080/opencv.js" type="text/javascript"></script>-->
   <script async src=" https://docs.opencv.org/master/opencv.js" type="text/javascript"></script>
</head>
<style>
  body { background-color: #808080;}    
  .column{
    float: left;
    width: 50%
  }
  .row:after{
    content: "";
    display: table;
    clear: both;
  }
</style>
<body>
<div class="container">
  <div class = "row"> 
    <div class = "column"> 
      <img id="ShowImage" src="" style="display:none">
      <canvas id="canvas" style="display:none"></canvas>
    
      <table>
      <tr>
      <td colspan="2"><input type="button" id="colorDetect" value="Color Detection" style="display:none"></td> 
      <td><input type="button" id="restart" value="Reset Board"></td> 
      </tr>  
      <tr>
        <td>TeleOp</td> 
        <td colspan="2">  
          <select id="mirrorimage">
            <option value="1">yes</option>
            <option value="0">no</option>
          </select>
        </td>
        <!--<td colspan="6">--> 
        <!--<td>SENSOR1</td>--> 
        <!--<td colspan="2">-->
        <!--<td colspan="2"><input type="slideOutput" id="SENSOR1" min="0" max="127" value="40"></td>-->     
        <!--</td>-->
        <td>
        <canvas id="textCanvas0" width="400" height="80" style= "border: 1px solid #black;"></canvas> 
        </td>
      </tr>
      <tr>
      <td>Quality</td>
      <td colspan="2"><input type="range" id="quality" min="10" max="63" value="10"></td>
      </tr>
      <tr>
      <td>Brightness</td>
      <td colspan="2"><input type="range" id="brightness" min="-2" max="2" value="0"></td>
      </tr>
      <tr>
      <td>Contrast</td>
      <td colspan="2"><input type="range" id="contrast" min="-2" max="2" value="0"></td>
      </tr>
      </table>
     
      <!-----ANN:5---->
      <div>
        <p>RGB COLOR TRACKBARS
      </div>

      <div class = "slidecontainer">
          <input type="range" id="rmin" min="0" max="255" 
              value="0" class = "slider">                 
          <input type="range" id="rmax" min="0" max="255" 
              value="50" class = "slider">
              <p>RMIN:<span id="RMINdemo"></span>
              &#160 &#160 &#160 &#160 &#160 &#160 &#160 &#160 &#160 &#160 &#160 
              RMAX:<span id="RMAXdemo"></span></p>                
      </div>

      <div class = "slidecontainer">
          <input type="range" id="gmin" min="0" max="255" 
              value="0" class = "slider">                 
          <input type="range" id="gmax" min="0" max="255" 
             value="50" class = "slider">                 
          <p>GMIN:<span id="GMINdemo"></span>
             &#160 &#160 &#160 &#160 &#160 &#160 &#160 &#160 &#160 &#160 &#160 
             GMAX:<span id="GMAXdemo"></span></p>
      </div>

      <div class = "slidecontainer">
          <input type="range" id="bmin" min="0" max="255" 
              value="0" class = "slider">                 
          <input type="range" id="bmax" min="0" max="255" 
              value="50" class = "slider">                 
          <p>BMIN:<span id="BMINdemo"></span>
             &#160 &#160 &#160 &#160 &#160 &#160 &#160 &#160 &#160 &#160 &#160 
             BMAX:<span id="BMAXdemo"></span></p>
      </div>

      <div>
        <p>THRESHHOLD MINIMUM-BINARY IMAGE
      </div>

      <div class = "slidecontainer">
          <input type="range" id="thresh_min" min="0" max="255" 
              value="120" class = "slider">                                
          <p>THRESH_MIN:<span id="THRESH_MINdemo"></span>
      </div>
     <!----ANN:9---> 
      <div>
        <p>COLOR PROBE
      </div>

      <div class = "slidecontainer">
          <input type="range" id="x_probe" min="0" max="400" 
              value="200" class = "slider">
          <input type="range" id="y_probe" min="0" max="296" 
              value="148" class = "slider">                                               
          <p>X_PROBE:<span id="X_PROBEdemo"></span>
             &#160 &#160 &#160 &#160 &#160 &#160 &#160 &#160 &#160 &#160 &#160 
             Y_PROBE:<span id="Y_PROBEdemo"></span>
             &#160 &#160 &#160 &#160 &#160 &#160 &#160 &#160 &#160 &#160 &#160 
             AJAX_INPUT:<span id="sw_an_data"></span></p>          
      </div>      

            
    </div>   <!------endfirstcolumn---------------->   
    
    <div class = "column">      
      <div>
      Image Mask
      </div>
      <div>
      <canvas id="imageMask"></canvas>
      </div>
      <div>
      Image Canvas
      </div>
      <div>
      <canvas id="imageCanvas"></canvas> 
      </div>

      <div class="list-group-item">
        <button type="button" id="invertButton" class="btn btn-primary">INVERT</button>
           <p>INVERT:<span id="INVERTdemo"></span></p>
      </div>
      <div class="list-group-item">
        <button type="button" id="contourButton" class="btn btn-primary">SHOW CONTOUR</button>
        &#160 &#160 &#160 &#160 &#160 &#160
        <button type="button" id="trackButton" class="btn btn-primary">TRACKING</button>
          <p>CONTOUR:<span id="CONTOURdemo"></span>&#160 &#160 &#160 &#160 &#160 &#160&#160 &#160 &#160 &#160 
             TRACK:<span id="TRACKdemo"></span>&#160 &#160 &#160 &#160 &#160 &#160 
             XCM:<span id="XCMdemo"></span>&#160 &#160 &#160 &#160 &#160 &#160
             YCM:<span id="YCMdemo"></span>&#160 &#160 &#160 &#160 &#160 &#160</p>
      </div>

      <div>
      <canvas id="textCanvas" width="480" height="180" style= "border: 1px solid #black;"></canvas> 
      </div>      

      <iframe id="ifr" style="display:none"></iframe>
      <div id="message" style="color:green"><div>
    </div>  <!------end2ndcolumn------------------------>
  </div>   <!-----endrow---------------------->   
</div>   <!------endcontainer-------------->

   
 <!--------------- </body>----------------->
 <!----------------</html>----------------->
<div class="modal"></div>
<script>
var sensor1 = document.getElementById("SENSOR1");
var colorDetect = document.getElementById('colorDetect');
var ShowImage = document.getElementById('ShowImage');
var canvas = document.getElementById("canvas");
var context = canvas.getContext("2d");
var mirrorimage = document.getElementById("mirrorimage"); 
var imageMask = document.getElementById("imageMask");
var imageMaskContext = imageMask.getContext("2d"); 
var imageCanvas = document.getElementById("imageCanvas");
var imageContext = imageCanvas.getContext("2d"); 
var txtcanvas = document.getElementById("textCanvas");
var txtcanvas0 = document.getElementById("textCanvas0");
var ctx = txtcanvas.getContext("2d"); 
var ctx0 = txtcanvas0.getContext("2d");  
var message = document.getElementById('message');
var ifr = document.getElementById('ifr');
var myTimer;
var restartCount=0;
const modelPath = 'https://ruisantosdotme.github.io/face-api.js/weights/';
let currentStream;
let displaySize = { width:400, height: 296 }
let faceDetection;
var ADCarr = 0;
var ans = [10,20];
var asensor = 20;
var bsensor = 0;
var ana_sensor1 = 0;
var ana_sensor2 = 0;
var dig_sensor1 = 0;
var dig_sensor2 = 0;


let b_tracker = false;
let b_tracker_int = 0;
let x_cm = 0;
let y_cm = 0;

var json_obj;

let b_invert = false;

let b_contour = false;

var RMAX=50;
var RMIN=0;
var GMAX=50;
var GMIN=0;
var BMAX=50;
var BMIN=0;
var THRESH_MIN=120;
var X_PROBE=200;
var Y_PROBE=196;
var R=0;
var G=0;
var B=0;
var A=0;


colorDetect.onclick = function (event) {
  clearInterval(myTimer);  
  myTimer = setInterval(function(){error_handle();},5000);
  ShowImage.src=location.origin+'/?colorDetect='+Math.random();
  //setInterval('GetSwitchAnalogData()', 1000);
}

//ANN:READY
var Module = {
  onRuntimeInitialized(){onOpenCvReady();}
}

function onOpenCvReady(){
  //alert("onOpenCvReady");
  console.log("OpenCV IS READY!!!");
  drawReadyText();  
  document.body.classList.remove("loading");
}

    
function error_handle() {
  restartCount++;
  clearInterval(myTimer);
  if (restartCount<=2) {
    message.innerHTML = "Get still error. <br>Restart ESP32-CAM "+restartCount+" times.";
    myTimer = setInterval(function(){colorDetect.click();},10000);
    ifr.src = document.location.origin+'?restart';
  }
  else
    message.innerHTML = "Get still error. <br>Please close the page and check ESP32-CAM.";
}    
colorDetect.style.display = "block";

ShowImage.onload = function (event) {
  //alert("SHOW IMAGE");
  console.log("SHOW iMAGE");
  clearInterval(myTimer);
  restartCount=0;      
  canvas.setAttribute("width", ShowImage.width);
  canvas.setAttribute("height", ShowImage.height);
  canvas.style.display = "block";

  if (mirrorimage.value==0) {
    //context.translate((canvas.width + ShowImage.width) / 2, 0);
    //context.scale(-1, 1);
    //context.drawImage(ShowImage, 0, 0, ShowImage.width, ShowImage.height);
    //context.drawImage(ShowImage, 400, 400, ShowImage.width, ShowImage.height);
    context.drawImage(ShowImage, 0, 0, ShowImage.width, ShowImage.height);
    //context.drawImage(ShowImage, 0, 0, 20,20);//SUPER SMALL
    //context.setTransform(1, 0, 0, 1, 0, 0);
    imageCanvas.setAttribute("width", ShowImage.width);
    imageCanvas.setAttribute("height", ShowImage.height);
    imageCanvas.style.display = "block";

    imageMask.setAttribute("width", ShowImage.width);
    imageMask.setAttribute("height", ShowImage.height);
    imageMask.style.display = "block";      

  }
  else {
    //context.drawImage(ShowImage,0,0,ShowImage.width,ShowImage.height);
    canvas.setAttribute("width", 1600);
    canvas.setAttribute("height", 800); 
    context.drawImage(ShowImage,0,0,1600,800);
    imageCanvas.setAttribute("width", 0);
    imageCanvas.setAttribute("height", 0);
    imageCanvas.style.display = "block";

    imageMask.setAttribute("width", 0);
    imageMask.setAttribute("height", 0);
    imageMask.style.display = "block";

           
  }      
    
  DetectImage();        
}
restart.onclick = function (event) {
  fetch(location.origin+'/?restart=stop');
}
quality.onclick = function (event) {
  fetch(document.location.origin+'/?quality='+this.value+';stop');
} 
brightness.onclick = function (event) {
  fetch(document.location.origin+'/?brightness='+this.value+';stop');
} 
contrast.onclick = function (event) {
  fetch(document.location.origin+'/?contrast='+this.value+';stop');
}                             
async function DetectImage() {
  //alert("DETECT IMAGE");
  console.log("DETECT IMAGE");
GetSwitchAnalogData();
asensor = asensor + 1;  

  /***************opencv********************************/
  //ANN:4
  let src = cv.imread(ShowImage);
  arows = src.rows;
  acols = src.cols;
  aarea = arows*acols;
  adepth = src.depth();
  atype = src.type();
  achannels = src.channels();
  console.log("rows = " + arows);
  console.log("cols = " + acols);
  console.log("pic area = " + aarea);
  console.log("depth = " + adepth); 
  console.log("type = " + atype); 
  console.log("channels = " + achannels);
  console.log("ASENSOR = " + asensor);
  console.log("BSENSOR = " + bsensor);   
  /******************COLOR DETECT******************************/

  //ANN:6  
  var RMAXslider = document.getElementById("rmax");
  var RMAXoutput = document.getElementById("RMAXdemo");
  RMAXoutput.innerHTML = RMAXslider.value;
  RMAXslider.oninput = function(){
  RMAXoutput.innerHTML = this.value;
  RMAX = parseInt(RMAXoutput.innerHTML,10);
  console.log("RMAX=" + RMAX);
  }

  console.log("RMAX=" + RMAX);

  var RMINslider = document.getElementById("rmin");
  var RMINoutput = document.getElementById("RMINdemo");
  RMINoutput.innerHTML = RMINslider.value;
  RMINslider.oninput = function(){
    RMINoutput.innerHTML = this.value;
    RMIN = parseInt(RMINoutput.innerHTML,10);
    console.log("RMIN=" + RMIN);
  }
  console.log("RMIN=" + RMIN);

  var GMAXslider = document.getElementById("gmax");
  var GMAXoutput = document.getElementById("GMAXdemo");
  GMAXoutput.innerHTML = GMAXslider.value;
  GMAXslider.oninput = function(){
    GMAXoutput.innerHTML = this.value;
    GMAX = parseInt(GMAXoutput.innerHTML,10);
  }
  console.log("GMAX=" + GMAX);

  var GMINslider = document.getElementById("gmin");
  var GMINoutput = document.getElementById("GMINdemo");
  GMINoutput.innerHTML = GMINslider.value;
  GMINslider.oninput = function(){
    GMINoutput.innerHTML = this.value;
    GMIN = parseInt(GMINoutput.innerHTML,10);
  }
  console.log("GMIN=" + GMIN);

  var BMAXslider = document.getElementById("bmax");
  var BMAXoutput = document.getElementById("BMAXdemo");
  BMAXoutput.innerHTML = BMAXslider.value;
  BMAXslider.oninput = function(){
    BMAXoutput.innerHTML = this.value;
    BMAX = parseInt(BMAXoutput.innerHTML,10);
  }
  console.log("BMAX=" + BMAX);

  var BMINslider = document.getElementById("bmin");
  var BMINoutput = document.getElementById("BMINdemo");
  BMINoutput.innerHTML = BMINslider.value;
  BMINslider.oninput = function(){
  BMINoutput.innerHTML = this.value;
  BMIN = parseInt(BMINoutput.innerHTML,10);
  }
  console.log("BMIN=" + BMIN);



  var THRESH_MINslider = document.getElementById("thresh_min");
  var THRESH_MINoutput = document.getElementById("THRESH_MINdemo");
  THRESH_MINoutput.innerHTML = THRESH_MINslider.value;
  THRESH_MINslider.oninput = function(){
  THRESH_MINoutput.innerHTML = this.value;
  THRESH_MIN = parseInt(THRESH_MINoutput.innerHTML,10);
  }
  console.log("THRESHOLD MIN=" + THRESH_MIN);

  //ANN:9A
  var X_PROBEslider = document.getElementById("x_probe");
  var X_PROBEoutput = document.getElementById("X_PROBEdemo");
  X_PROBEoutput.innerHTML = X_PROBEslider.value;
  X_PROBEslider.oninput = function(){
  X_PROBEoutput.innerHTML = this.value;
  X_PROBE = parseInt(X_PROBEoutput.innerHTML,10);
  }
  console.log("X_PROBE=" + X_PROBE); 

  var Y_PROBEslider = document.getElementById("y_probe");
  var Y_PROBEoutput = document.getElementById("Y_PROBEdemo");
  Y_PROBEoutput.innerHTML = Y_PROBEslider.value;
  Y_PROBEslider.oninput = function(){
  Y_PROBEoutput.innerHTML = this.value;
  Y_PROBE = parseInt(Y_PROBEoutput.innerHTML,10);
  }
  console.log("Y_PROBE=" + Y_PROBE); 

  var ZETA = document.getElementById("sw_an_data");
  //ZETA.innerHTML = "ON"; //TEST 

  document.getElementById('trackButton').onclick = function(){
    b_tracker = (true && !b_tracker) 
    if(b_tracker==true){
      b_tracker_int = 1; 
    }
    if(b_tracker==false){
      b_tracker_int = 0;
    }
    console.log("TRACKER = " + b_tracker );
    console.log("TRACKER INT = " + b_tracker_int );
    var TRACKoutput = document.getElementById("TRACKdemo");
    TRACKoutput.innerHTML = b_tracker;
    //var XCMoutput = document.getElementById("XCMdemo");
    //XCMoutput.innerHTML = x_cm;
 
  }  

  document.getElementById('invertButton').onclick = function(){
    b_invert = (true && !b_invert)  
    console.log("TRACKER = " + b_invert );
    var INVERToutput = document.getElementById("INVERTdemo");
    INVERToutput.innerHTML = b_invert;
  }  
/**/
  document.getElementById('contourButton').onclick = function(){
    b_contour = (true && !b_contour)  
    console.log("TRACKER = " + b_contour );
    var CONTOURoutput = document.getElementById("CONTOURdemo");
    CONTOURoutput.innerHTML = b_contour;
  } 
/**/ 

  let tracker = 0;
  
  var TRACKoutput = document.getElementById("TRACKdemo");
  TRACKoutput.innerHTML = b_tracker;
  var XCMoutput = document.getElementById("XCMdemo");
  var YCMoutput = document.getElementById("YCMdemo");

  XCMoutput.innerHTML = 0;
  YCMoutput.innerHTML = 0; 

  var INVERToutput = document.getElementById("INVERTdemo");
  INVERToutput.innerHTML = b_invert;  

  var CONTOURoutput = document.getElementById("CONTOURdemo");
  CONTOURoutput.innerHTML = b_contour;   

  //ANN:8
  let M00Array = [0,];
  let orig = new cv.Mat();
  let mask = new cv.Mat();
  let mask1 = new cv.Mat();
  let mask2 = new cv.Mat();
  let contours = new cv.MatVector();
  let hierarchy = new cv.Mat();
  let rgbaPlanes = new cv.MatVector();
    
  let color = new cv.Scalar(0,0,0);

  clear_canvas();
  clear_canvas0();

    
  orig = cv.imread(ShowImage);
  cv.split(orig,rgbaPlanes);  //SPLIT
  let BP = rgbaPlanes.get(2);  // SELECTED COLOR PLANE
  let GP = rgbaPlanes.get(1);
  let RP = rgbaPlanes.get(0);
  cv.merge(rgbaPlanes,orig);
   
    
              //   BLK    BLU   GRN   RED
  let row = Y_PROBE //180//275 //225 //150 //130    
  let col = X_PROBE //100//10 //100 //200 //300
  drawColRowText(acols,arows);
  //drawASensorText(asensor,bsensor);
  drawASensorText(ana_sensor1,dig_sensor1,ana_sensor2,dig_sensor2);

  console.log("ISCONTINUOUS = " + orig.isContinuous());

  //console.log("TRACKER = " + b_tracker );
  console.log("TRACKER INT = " + b_tracker_int );

  //ANN:9C
  R = src.data[row * src.cols * src.channels() + col * src.channels()];
  G = src.data[row * src.cols * src.channels() + col * src.channels() + 1];
  B = src.data[row * src.cols * src.channels() + col * src.channels() + 2];
  A = src.data[row * src.cols * src.channels() + col * src.channels() + 3];
  console.log("RDATA = " + R);
  console.log("GDATA = " + G);
  console.log("BDATA = " + B);
  console.log("ADATA = " + A);

  drawRGB_PROBE_Text();
  
   
    
  //ANN:9b
  //*************draw probe point*********************
  let point4 = new cv.Point(col,row);
  cv.circle(src,point4,5,[255,255,255,255],2,cv.LINE_AA,0);
  //***********end draw probe point*********************

  //ANN:7
  let high = new cv.Mat(src.rows,src.cols,src.type(),[RMAX,GMAX,BMAX,255]);
  let low = new cv.Mat(src.rows,src.cols,src.type(),[RMIN,GMIN,BMIN,0]);

  cv.inRange(src,low,high,mask1);
  //inRange(source image, lower limit, higher limit, destination image)
    
  cv.threshold(mask1,mask,THRESH_MIN,255,cv.THRESH_BINARY);
  //threshold(source image,destination image,threshold,255,threshold method);

  //ANN:9
  if(b_invert==true){
     cv.bitwise_not(mask,mask2);
  }
  
  //dataTransmit2(5);
  
  //GetSwitchAnalogData();
  console.log("ADCARR = " + ADCarr);
  
 
   
/********************start contours******************************************/
  //ANN:10
  if(b_tracker == true){
  try{ 
   if(b_invert==false){
    //ANN:11   
    cv.findContours(mask,contours,hierarchy,cv.RETR_CCOMP,cv.CHAIN_APPROX_SIMPLE);
    //findContours(source image, array of contours found, hierarchy of contours
        // if contours are inside other contours, method of contour data retrieval,
        //algorithm method)
   }
   else{
    cv.findContours(mask2,contours,hierarchy,cv.RETR_CCOMP,cv.CHAIN_APPROX_SIMPLE);
   }
    console.log("CONTOUR_SIZE = " + contours.size());

    //draw contours
    if(b_contour==true){
     for(let i = 0; i < contours.size(); i++){
        cv.drawContours(src,contours,i,[0,0,0,255],2,cv.LINE_8,hierarchy,100)
     }
    }

    //ANN:12
    let cnt;
    let Moments;
    let M00;
    let M10;
    //let x_cm;
    //let y_cm;
    
    //ANN:13
    for(let k = 0; k < contours.size(); k++){
        cnt = contours.get(k); 
        Moments = cv.moments(cnt,false);
        M00Array[k] = Moments.m00;
       // cnt.delete();
    }

    //ANN13A
    let max_area_arg = MaxAreaArg(M00Array);
    console.log("MAXAREAARG = "+max_area_arg);

    //let TestArray = [0,0,0,15,4,15,2];
    //let TestArray0 = [];
    //let max_test_area_arg = MaxAreaArg(TestArray0);
    //console.log("MAXTESTAREAARG = "+max_test_area_arg);



    let ArgMaxArea = MaxAreaArg(M00Array);
    if(ArgMaxArea >= 0){
    cnt = contours.get(MaxAreaArg(M00Array));  //use the contour with biggest MOO
    //cnt = contours.get(54);
    Moments = cv.moments(cnt,false);
    M00 = Moments.m00;
    M10 = Moments.m10;
    M01 = Moments.m01;
    x_cm = M10/M00;    // 75 for circle_9.jpg
    y_cm = M01/M00;    // 41 for circle_9.jpg

    XCMoutput.innerHTML = Math.round(x_cm);
    YCMoutput.innerHTML = Math.round(y_cm);

    console.log("M00 = "+M00);  
    console.log("XCM = "+Math.round(x_cm));
    console.log("YCM = "+Math.round(y_cm));
     

    //fetch(document.location.origin+'/?xcm='+Math.round(x_cm)+';stop');
    //EXPT TEMPORARY OMIT THIS NEXT FETCH. NO EFFECT. PUT BACK
    //fetch(document.location.origin+'/?cm='+Math.round(x_cm)+';'+Math.round(y_cm)+';stop');
    // original//fetch(document.location.origin+'/?cm='+Math.round(x_cm)+';'+Math.round(y_cm)+';'+b_tracker_int+';stop');
    //dataTransmit();
    //dataTransmit2(5);
    //GetSwitchAnalogData();
    console.log("ADCARR = " + ADCarr);
    console.log("M00ARRAY = " + M00Array);

    //ANN:14   
    
    //**************min area bounding rect********************
    //let rotatedRect=cv.minAreaRect(cnt);
    //let vertices = cv.RotatedRect.points(rotatedRect);

    //for(let j=0;j<4;j++){
    //    cv.line(src,vertices[j],
    //        vertices[(j+1)%4],[0,0,255,255],2,cv.LINE_AA,0);
    //}
    //***************end min area bounding rect*************************************


    //***************bounding rect***************************
    let rect = cv.boundingRect(cnt);
    let point1 = new cv.Point(rect.x,rect.y);
    let point2 = new cv.Point(rect.x+rect.width,rect.y+rect.height);

    cv.rectangle(src,point1,point2,[0,0,255,255],2,cv.LINE_AA,0);
    //*************end bounding rect***************************


    //*************draw center point*********************
    let point3 = new cv.Point(x_cm,y_cm);
    cv.circle(src,point3,2,[0,0,255,255],2,cv.LINE_AA,0);
    //***********end draw center point*********************

    }//end if(ArgMaxArea >= 0)
    else{
      if(ArgMaxArea==-1){ 
        console.log("ZERO ARRAY LENGTH");
      }
      else{              //ArgMaxArea=-2
        console.log("DUPLICATE MAX ARRAY-ELEMENT");
      }
    }




    cnt.delete();
/******************end contours  note cnt line one up*******************************************/
   drawXCM_YCM_Text();

  }//end try
  catch{
    console.log("ERROR TRACKER NO CONTOUR");
    clear_canvas();
    clear_canvas0();
    drawErrorTracking_Text();
  }
    
  }//end b_tracking if statement
  else{
      XCMoutput.innerHTML = 0;
      YCMoutput.innerHTML = 0;
      fetch(document.location.origin+'/?cm='+Math.round(0)+';'+Math.round(0)+';'+b_tracker_int+';stop');
  }    
  if(mirrorimage.value==0){
   if(b_invert==false){
      cv.imshow('imageMask', mask);
   }
   else{
      cv.imshow('imageMask', mask2);
   }
   //cv.imshow('imageMask', R);
   cv.imshow('imageCanvas', src);
  }
  //ANN:8A
  src.delete();
  high.delete();
  low.delete();
  orig.delete();
  mask1.delete();
  mask2.delete();
  mask.delete();
  contours.delete();
  hierarchy.delete();
  //cnt.delete();
  RP.delete();
    
  


 /********************END COLOR DETECT****************************/
  
/***************end opencv******************************/
      

 setTimeout(function(){colorDetect.click();},500);
  
}//end detectimage 


function MaxAreaArg(arr){
    if (arr.length == 0) {
        return -1;
    }

    var max = arr[0];
    var maxIndex = 0;
    var dupIndexCount = 0; //duplicate max elements?

    if(arr[0] >= .90*aarea){
        max = 0;
    }

    for (var i = 1; i < arr.length; i++) {
        if (arr[i] > max && arr[i] < .99*aarea) {
            maxIndex = i;
            max = arr[i];
            dupIndexCount = 0;
        }
        else if(arr[i]==max && arr[i]!=0){
            dupIndexCount++;
        }
    }

    if(dupIndexCount==0){
        return maxIndex;
    }

    else{
        return -2;
    }        
}//end MaxAreaArg    




function clear_canvas(){
  if (mirrorimage.value==0){
    ctx.clearRect(0,0,txtcanvas.width,txtcanvas.height);
    ctx.rect(0,0,txtcanvas.width,txtcanvas.height);
    ctx.fillStyle="green";
    ctx.fill();
  }
  else{
    ctx.clearRect(0,0,txtcanvas.width,txtcanvas.height);
    ctx.rect(0,0,txtcanvas.width,txtcanvas.height);
    //ctx.fillStyle="white";
    //ctx.fill();      
  }
}

function clear_canvas0(){
  if (mirrorimage.value==1){
    ctx0.clearRect(0,0,txtcanvas0.width,txtcanvas.height);
    ctx0.rect(0,0,txtcanvas0.width,txtcanvas0.height);
    ctx0.fillStyle="red";
    ctx0.fill();
  }
  else{
    ctx0.clearRect(0,0,txtcanvas0.width,txtcanvas0.height);
    ctx0.rect(0,0,txtcanvas0.width,txtcanvas0.height);
    //ctx.fillStyle="white";
    //ctx.fill();      
  }
}

function drawASensorText(x,y,w,z){
  if(mirrorimage.value==0){
    ctx.fillStyle = 'black';
    ctx.font = '20px serif';
    ctx.fillText('ANA_SENSOR1_OCV = '+x,0,4*txtcanvas.height/10);
    ctx.fillText('DIG_SENSOR1_OCV = '+y,txtcanvas.width/2,4*txtcanvas.height/10);
    ctx.fillText('ANA_SENSOR2_OCV = '+w,0,6*txtcanvas.height/10);
    ctx.fillText('DIG_SENSOR2_OCV = '+z,txtcanvas.width/2,6*txtcanvas.height/10);
  }
  if(mirrorimage.value==1){
    ctx0.fillStyle = 'black';
    ctx0.font = '20px serif';
    ctx0.fillText('ANA_SENSOR1 = '+x,0,2*txtcanvas0.height/10);
    ctx0.fillText('DIG_SENSOR1 = '+y,txtcanvas0.width/2,2*txtcanvas0.height/10);
    ctx0.fillText('ANA_SENSOR2 = '+w,0,6*txtcanvas0.height/10);
    ctx0.fillText('DIG_SENSOR2 = '+z,txtcanvas0.width/2,6*txtcanvas0.height/10);
  }  
}

function drawReadyText(){
    ctx.fillStyle = 'black';
    ctx.font = '20px serif';
    ctx.fillText('OpenCV.JS READY',txtcanvas.width/4,txtcanvas.height/10);
}          

function drawColRowText(x,y){
  if (mirrorimage.value==0){
    ctx.fillStyle = 'black';
    ctx.font = '20px serif';
    ctx.fillText('ImageCols='+x,0,txtcanvas.height/10);
    ctx.fillText('ImageRows='+y,txtcanvas.width/2,txtcanvas.height/10);
  }  
} 

function drawRGB_PROBE_Text(){
  if (mirrorimage.value==0){
    ctx.fillStyle = 'black';
    ctx.font = '20px serif';
    ctx.fillText('Rp='+R,0,2*txtcanvas.height/10);
    ctx.fillText('Gp='+G,txtcanvas.width/4,2*txtcanvas.height/10);
    ctx.fillText('Bp='+B,txtcanvas.width/2,2*txtcanvas.height/10);
    ctx.fillText('Ap='+A,3*txtcanvas.width/4,2*txtcanvas.height/10);
  }  
}

function drawXCM_YCM_Text(){
    ctx.fillStyle = 'black';
    ctx.font = '20px serif';
    ctx.fillText('XCM='+Math.round(x_cm),0,3*txtcanvas.height/10); 
    ctx.fillText('YCM='+Math.round(y_cm),txtcanvas.width/4,3*txtcanvas.height/10);    
}

function drawErrorTracking_Text(){
    ctx.fillStyle = 'black';
    ctx.font = '20px serif';
    ctx.fillText('ERROR TRACKING-NO CONTOUR',0,3*txtcanvas.height/10);
}

//COPY FROM TTGO_T_JOURNAL_ROBOT_WEB_21
function dataTransmit() {
     alert("DATA_TRANSMIT");
     var xhr = new XMLHttpRequest();
     xhr.onreadystatechange = function() {
     console.log("READYSTATE===="+this.readyState);
     console.log("STATUS===="+this.status);    
     if(this.readyState==4 && this.status == 200){
          //ADC = xhr.responseText;
         //ADC = xhr.response;
         console.log("RESPONSE == "+xhr.response);
         ADCarr = xhr.response;
      }
     };
     
     
     //xhr.open("GET", "/action?go=" + x, true);
     xhr.open("GET",document.location.origin+'/?cm='+Math.round(x_cm)+';'+Math.round(y_cm)+';'+b_tracker_int+';stop',true);
     xhr.send();     
}  

//ANN:15
//NEW DATATRANSMIT
  function GetSwitchAnalogData() {
    nocache = "&nocache=" + Math.random() * 1000000;
    var request = new XMLHttpRequest();
    //document.getElementById("sw_an_data").innerHTML = "ON"; //TEST
    request.onreadystatechange = function() {
      console.log("READYSTATE===="+this.readyState);
      console.log("STATUS===="+this.status);
      if (this.readyState == 4) {        
        if(this.status == 200) {
          if(this.responseText != null) {
            document.getElementById("sw_an_data").innerHTML = this.responseText;
            console.log("RESPONSETEXT = "+this.responseText);
            //json_obj = JSON.parse(this.responseText);
            //console.log("DIGDATA = "+json_obj.SWITCH_STATE);
            //console.log("ANADATA = "+json_obj.ANALOG);
            json_obj = JSON.parse(this.responseText);
            dig_sensor1 = json_obj[0].DIGSENSOR1;
            dig_sensor2 = json_obj[0].DIGSENSOR2;
            ana_sensor1 = json_obj[1].ANASENSOR1;
            ana_sensor2 = json_obj[1].ANASENSOR2;
            console.log("DIGDATA1 = "+json_obj[0].DIGSENSOR1);
            console.log("DIGDATA2 = "+json_obj[0].DIGSENSOR2);
            console.log("ANADATA1 = "+json_obj[1].ANASENSOR1);
            console.log("ANADATA2 = "+json_obj[1].ANASENSOR2);
          }//end responseText
        }//end status
      }//end readyState
    }// end inner func
    //request.open("GET", "ajax_switch" + nocache, true);
    //request.open("GET", "ajax_switch", true);
    request.open("GET",document.location.origin+'/?cm='+Math.round(x_cm)+';'+Math.round(y_cm)+';'+b_tracker_int+';stop',true);    
    request.send(null);
  }//end func
//END NEW DATATRANSMIT          
    
  </script> 
  </body>
  </html>  
)rawliteral";

