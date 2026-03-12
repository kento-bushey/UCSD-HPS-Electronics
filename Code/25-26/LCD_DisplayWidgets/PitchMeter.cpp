#include "PitchMeter.h"

void PitchMeter::InitPitchMeter(float radius_, float fov_, float yScale_){
  radius = radius_;
  tfov2 = tan(fov_/2.0);
  yScale = yScale_;
}

void PitchMeter::Draw(int pitch, uint16_t color1, uint16_t color2, uint16_t color3) {
  //TODO: this is mostly processing4 code, it needs to be translated properly into c++ with lcd libraries 
    for(int i = pitch-15; i<=pitch+15; i++){
    float z = -radius*cos( radians( yScale*abs(static_cast<float>(i-pitch)) ) );
      //println(z);
      float y = radius*(sin( radians( yScale*static_cast<float>(i-pitch) ) ));
      
     
     float wh = w*radius/(z*tfov2);
     float yh = h*y/(z*tfov2);
    
     
     
    if(abs(i)%5==0){
      
        if(abs(i)%10==0){
           //if(i<0)stroke(100,100,255);
           //else stroke(100,255,100);
           //if(i==0){strokeWeight(4); stroke(255,255,0);}
           //if(i==pitch){strokeWeight(4); stroke(255,0,0);}
           screen->drawLine(
                pos.x-wh, pos.y+yh,
                pos.x+wh, pos.y+yh, color1
           );
           /*TODO::
            * 
           textSize(30);
           text(i,width/2+1.2*wh,height/2+yh+10);
           strokeWeight(2);
           */
        }else{
           //stroke(255);
           //if(i==0){strokeWeight(4); stroke(255,255,0);}
           //if(i==pitch){strokeWeight(4); stroke(255,0,0);}
           screen->drawLine(
                pos.x-wh*0.5f, pos.y+yh,
                pos.x+wh*0.5f, pos.y+yh, color2
           );

           
           /*TODO::
            * textSize(20);
           text(i,width/2+wh*0.5f-30,height/2+yh+5);
           strokeWeight(2);
           */
        }
      }
      else{
          //stroke(100);
          screen->drawLine(
                pos.x-wh*0.025f, pos.y+yh,
                pos.x+wh*0.025f, pos.y+yh, color2
           );
     }
  
  }
}
