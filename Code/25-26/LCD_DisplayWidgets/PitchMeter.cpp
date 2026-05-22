#include "PitchMeter.h"

void PitchMeter::InitPitchMeter(float dist, float radius_, float fov_, float angleScale_, float imgScale_){
  radius = radius_;
  tfov2 = tan(radians(fov_)/2.0);
  angleScale = angleScale_;
  distToCenter = dist;
  imgScale=imgScale_;
}

void PitchMeter::DrawBackground(uint16_t color1, uint16_t color2, uint16_t color3){
    //Bounding box
    screen->drawRect(pos.x,pos.y,w,h,color1);
    screen->drawCircle(pos.x+w*0.5,pos.y+h*0.5,(imgScale*radius/2)*w/(tfov2*(distToCenter)), color1);

    screen->drawChar(pos.x,pos.y, 'P', color1,0,2);
    screen->drawChar(pos.x+12,pos.y, 'i', color1,0,2);
    screen->drawChar(pos.x+22,pos.y, 't', color1,0,2);
    screen->drawChar(pos.x+36,pos.y, 'c', color1,0,2);
    screen->drawChar(pos.x+48,pos.y, 'h', color1,0,2);

}

void PitchMeter::Draw(int pitch, uint16_t color1, uint16_t color2, uint16_t color3) {
      screen->drawLine(pos.x+w/2 + w/10,pos.y+h*0.5f,pos.x+w/2 - w/10,pos.y+0.5*h,color1);
     screen->drawLine(pos.x+w/2 ,pos.y+h*0.5f+h/20,pos.x+w/2,pos.y+0.5*h-h/20,color1);
    //loop over 20 degrees of pitch:
    for(int i = pitch-10; i<=pitch+10; i++){

    
    float z = distToCenter + radius*cos( radians( angleScale*abs(static_cast<float>(i-pitch)) ) );
    float y = radius*(sin( radians( angleScale*static_cast<float>(i-pitch) ) ));
    float x_off = radius/2;
    float y_ends = ( angleScale*abs(static_cast<float>(i-pitch)) ) * (i-pitch>0?-1.0:1.0);
    y_ends*=0.95f*h/(z*tfov2);

     float wh = x_off*w/(z*tfov2);
     float yh = -0.5*h*y/(z*tfov2);
    
     
     
    if(abs(i)%5==0){
      
        if(abs(i)%10==0){
        
           

          screen->drawBezier(
            {pos.x+w/2 + imgScale*wh,   pos.y+h/2 + imgScale*y_ends}, 
            {pos.x+w/2 - imgScale*wh,   pos.y+h/2 + imgScale*y_ends}, 
            {pos.x+w/2     ,   pos.y+h/2 + 2.0*imgScale*yh - imgScale*y_ends}, 
            10,  
            i>0?color2:color3
          );


           float textY = constrain(pos.y+h/2+imgScale*yh, pos.y,pos.y+h-10);
           screen->drawChar(pos.x+w*0.5f+10,  textY,  '0' + (char)(abs(i)%10),   color1, 0, abs(sin( radians( angleScale*static_cast<float>(i-pitch) ) ))<0.45?2:1);
           screen->drawChar(pos.x+w*0.5f-5,   textY,  '0' + (char)(abs(i)/10), color1, 0, abs(sin( radians( angleScale*static_cast<float>(i-pitch) )) )<0.45?2:1);
           screen->drawChar(pos.x+w*0.5f-20,  textY,  i<0?'-':(i>0?'+':' '), color1, 0, abs(sin( radians( angleScale*static_cast<float>(i-pitch) ) ))<0.45?2:1);
           
        }else{
          
         screen->drawBezier(
            {pos.x+w/2 + imgScale*wh,   pos.y+h/2 + imgScale*y_ends}, 
            {pos.x+w/2 - imgScale*wh,   pos.y+h/2 + imgScale*y_ends}, 
            {pos.x+w/2     ,   pos.y+h/2 + 2.0*imgScale*yh - imgScale*y_ends}, 
            8,  
            i > 0 ? color1&0b0000111011111111 : color1&0b1101100011111100
          );

        }
      }
      else{
          //stroke(100);
          /*if(i%2==0){screen->drawLine(
                pos.x-wh*0.1f + w*0.5f, pos.y+yh + h*0.5f,
                pos.x+wh*0.1f + w*0.5f, pos.y+yh + h*0.5f, color3
           );*/
          //}
     }

  
  }
  
   
     //float textY = pos.y+h/2;
    //  screen->drawChar(pos.x+w*0.5f+15,  textY,  '0' + (char)(abs(pitch)%10),   color1, 0, 3);
     // screen->drawChar(pos.x+w*0.5f-5,   textY,  '0' + (char)(abs(pitch)/10), color1, 0, 3);
     /// screen->drawChar(pos.x+w*0.5f-25,  textY,  pitch>0?'-':(pitch>0?'+':' '), color3, color1, 3);
  previousValue = pitch;
}
