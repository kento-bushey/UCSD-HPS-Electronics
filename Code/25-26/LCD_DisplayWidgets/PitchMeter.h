#include "Widget.h"

class PitchMeter : public Widget{

  public:
    //Use parent constructor and Draw methods
    using Widget::Widget;
    using Widget::Draw;

    void InitPitchMeter(float dist, float radius_, float fov_, float angleScale_, float imgScale_);
    void Draw(int pitch, uint16_t color1,uint16_t color2, uint16_t color3) override;
    void DrawBackground(uint16_t color1,uint16_t color2, uint16_t color3) override;

  private:
    float radius;
    float distToCenter;
    float tfov2;
    float angleScale;
    float imgScale;
  
};
