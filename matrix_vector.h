#pragma once


struct vec2 {
  float x, y;
};

struct vec3 {
  float x, y, z;
};

struct int_vec2 {
  int x, y;
};

struct vec4 {
  float x, y, z, w;

  int_vec2 to_int() {
    int_vec2 c;

    c.x = this->x;
    c.y = this->y;
    return c;
  }
};




struct f_matrix {
  float m[16] = {0.0f};

  inline void setZero() {
    memset(m, 0, 16 * sizeof(float));
  }

  inline void setPerspective(float fov, float zNear, float zFar, float screen_size) {
    float tan_half_fov = tan(fov * 0.5f * PI / 180);
    float range_inv = 1 / (zFar - zNear);

    m[0] = (screen_size / tan_half_fov);
    m[5] = (screen_size / tan_half_fov);
    m[10] = -(zFar + zNear) * range_inv;
    m[11] = -2.0f * zFar * zNear * range_inv;
    m[14] = -1.0f;
  }

  inline void setRotateX(float angle) {
    float sin_half_angle = sin(angle / 2.0f);
    float cos_half_angle = cos(angle / 2.0f);

    m[0] = 1.0f;
    m[5] = cos_half_angle;
    m[6] = sin_half_angle;
    m[9] = -sin_half_angle;
    m[10] = cos_half_angle;
    m[15] = 1.0f;
  }

  inline void setRotateY(float angle) {
    float sin_half_angle = sin(angle / 2.0f);
    float cos_half_angle = cos(angle / 2.0f);

    
    m[0] = cos_half_angle;
    m[2] = -sin_half_angle;
    m[5] = 1.0f;
    m[8] = sin_half_angle;
    m[10] = cos_half_angle;
    m[15] = 1.0f;
  }

  inline void setRotateZ(float angle) {
    float sin_half_angle = sin(angle / 2.0f);
    float cos_half_angle = cos(angle / 2.0f);

    
    m[0] = cos_half_angle;
    m[1] = sin_half_angle;
    m[4] = -sin_half_angle;
    m[5] = cos_half_angle;
    m[10] = 1.0f;
    m[15] = 1.0f;
  }

  inline void setScale(float x, float y, float z) {
    m[0] = x;
    m[5] = y;
    m[10] = z;
    m[15] = 1.0f;
  }

  inline void setTranslate(float x, float y, float z) {
    m[0] = 1.0f;
    m[5] = 1.0f;
    m[10] = 1.0f;
    m[15] = 1.0f;
    m[3] = x;
    m[7] = y;
    m[11] = z;
  }

  inline f_matrix operator*(const f_matrix& other) const {
    f_matrix result;

    for (int y = 0; y < 16; y += 4) 
      for (int x = 0; x < 4; ++x) 
        result.m[x + y] = 
            m[y] * other.m[x] + 
            m[1 + y] * other.m[x + 4] + 
            m[2 + y] * other.m[x + 8] + 
            m[3 + y] * other.m[x + 12];
    return result;
  }

  inline vec4 operator*(const vec3& v) const {
    return (vec4) {
      v.x * m[0] + 
      v.y * m[1] + 
      v.z * m[2] + 
      m[3], 

			v.x * m[4] + 
      v.y * m[5] + 
      v.z * m[6] + 
      m[7], 

			v.x * m[8] + 
      v.y * m[9] + 
      v.z * m[10] + 
      m[11],

			v.x * m[12] + 
      v.y * m[13] + 
      v.z * m[14] + 
      m[15]
    };
  }

  inline void multPerspective(float fov, float zNear, float zFar, float screen_size) {
    f_matrix M;
    M.setPerspective(fov, zNear, zFar, screen_size);
    *this = (*this) * M;
  }

  inline void multRotateX(float angle) {
    f_matrix M;
    M.setRotateX(angle);
    *this = (*this) * M;
  }

  inline void multRotateY(float angle) {
    f_matrix M;
    M.setRotateY(angle);
    *this = (*this) * M;
  }

  inline void multRotateZ(float angle) {
    f_matrix M;
    M.setRotateZ(angle);
    *this = (*this) * M;
  }

  inline void multTranslate(float x, float y, float z) {
    f_matrix M;
    M.setTranslate(x, y, z);
    *this = (*this) * M;
  }

  inline void multScale(float x, float y, float z) {
    f_matrix M;
    M.setScale(x, y, z);
    *this = (*this) * M;
  }
};
