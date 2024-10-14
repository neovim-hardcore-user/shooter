#pragma once


struct vec2 {
  int x, y;
};

struct vec3 {
  int x, y, z;
};

struct vec4 {
  int x, y, z, w;
};

float FIXED_POINT = pow(2, 16);

struct f_matrix {
  int m[16] = {0};

  inline void setZero() {
    memset(m, 0, 16 * sizeof(int));
  }

  inline void setPerspective(float fov, float zNear, float zFar, float screen_size) {
    float tan_half_fov = tan(fov * 0.5f * PI / 180);
    float range_inv = 1 / (zFar - zNear);

    m[0] = (screen_size / tan_half_fov) * FIXED_POINT;
    m[5] = (screen_size / tan_half_fov) * FIXED_POINT;
    m[10] = -(zFar + zNear) * range_inv * FIXED_POINT;
    m[11] = -2.0f * zFar * zNear * range_inv * FIXED_POINT;
    m[14] = -(1 << 16);
  }

  inline void setRotateX(float angle) {
    float angle_radians = angle * PI / 180.0f;

    int sin_half_angle = sin(angle_radians / 2.0) * FIXED_POINT;
    int cos_half_angle = cos(angle_radians / 2.0) * FIXED_POINT;

    m[0] = 1<<16;
    m[5] = cos_half_angle;
    m[6] = sin_half_angle;
    m[9] = -sin_half_angle;
    m[10] = cos_half_angle;
    m[15] = 1<<16;
  }

  inline void setRotateY(float angle) {
    float angle_radians = angle * PI / 180.0f;

    int sin_half_angle = sin(angle_radians / 2.0) * FIXED_POINT;
    int cos_half_angle = cos(angle_radians / 2.0) * FIXED_POINT;

    
    m[0] = cos_half_angle;
    m[2] = -sin_half_angle;
    m[5] = 1<<16;
    m[8] = sin_half_angle;
    m[10] = cos_half_angle;
    m[15] = 1<<16;
  }

  inline void setRotateZ(float angle) {
    float angle_radians = angle * PI / 180.0f;

    int sin_half_angle = sin(angle_radians / 2.0) * FIXED_POINT;
    int cos_half_angle = cos(angle_radians / 2.0) * FIXED_POINT;

    
    m[0] = cos_half_angle;
    m[1] = sin_half_angle;
    m[4] = -sin_half_angle;
    m[5] = cos_half_angle;
    m[10] = 1<<16;
    m[15] = 1<<16;
  }

  inline void setScale(float x, float y, float z) {
    m[0] = x * FIXED_POINT;
    m[5] = y * FIXED_POINT;
    m[10] = z * FIXED_POINT;
    m[16] = 1 << 16;
  }

  inline void setTranslate(float x, float y, float z) {
    m[0] = 1<<16;
    m[5] = 1<<16;
    m[10] = 1<<16;
    m[15] = 1<<16;
    m[3] = x * FIXED_POINT;
    m[7] = y * FIXED_POINT;
    m[11] = z * FIXED_POINT;
  }

  inline f_matrix operator*(const f_matrix& other) const {
    f_matrix result;

    for (int y = 0; y < 4; ++y) 
      for (int x = 0; x < 4; ++x) 
        result.m[x + (y << 2)] = 
            int32_t((int64_t(m[y << 2]) * int64_t(other.m[x])) >> 16) + 
            int32_t((int64_t(m[1 + (y << 2)]) * int64_t(other.m[x + 4])) >> 16) + 
            int32_t((int64_t(m[2 + (y << 2)]) * int64_t(other.m[x + 8])) >> 16) + 
            int32_t((int64_t(m[3 + (y << 2)]) * int64_t(other.m[x + 12])) >> 16);
    return result;
  }

  inline vec4 operator*(const vec3& v) const {
    return (vec4) {
      (int32_t(v.x * int64_t(m[0]) >> 16)) + 
      (int32_t(v.y * int64_t(m[1]) >> 16)) + 
      (int32_t(v.z * int64_t(m[2]) >> 16)) + 
      m[3], 

			(int32_t(v.x * int64_t(m[4]) >> 16)) + 
      (int32_t(v.y * int64_t(m[5]) >> 16)) + 
      (int32_t(v.z * int64_t(m[6]) >> 16)) + 
      m[7], 

			(int32_t(v.x * int64_t(m[8]) >> 16)) + 
      (int32_t(v.y * int64_t(m[9]) >> 16)) + 
      (int32_t(v.z * int64_t(m[10]) >> 16)) + 
      m[11],

			(int32_t(v.x * int64_t(m[12]) >> 16)) + 
      (int32_t(v.y * int64_t(m[13]) >> 16)) + 
      (int32_t(v.z * int64_t(m[14]) >> 16)) + 
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
