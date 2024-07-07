#include <math/matrix.h>
#include <math/vector.h>

namespace fps::rendering {
   using namespace fps::math;

   template<typename NumericType>
   class basic_fps_camera {
      vec3<NumericType> position_{ 0, 0, 0 };
      NumericType yaw_{ 0 };
      NumericType pitch_{ 0 };
      vec4f up_;
      vec4f right_;
      vec4f backward_;
      matrix rot_matrix_;
      matrix transl_matrix_;
      mutable matrix viewmatrix_;
      mutable bool bDirty_;

      static inline constexpr auto Zero = NumericType{0};
      static inline constexpr auto One = NumericType{1};

   public:
      constexpr basic_fps_camera(vec3<NumericType> const& pos, NumericType yaw, NumericType pitch)
         : position_{ pos },
         yaw_{ yaw },
         pitch_{ pitch },
         bDirty_{ true }
      {
         calc_rot_matrix();
         calc_transl_matrix();
      }

      constexpr auto yaw() const noexcept { return yaw_; }
      constexpr auto pitch() const noexcept { return pitch_; }
      constexpr auto const& position() const noexcept { return position_; }
      constexpr auto dirty() const noexcept { return bDirty_; }

      constexpr matrix view_matrix() const noexcept {
         if (not bDirty_) {
            return viewmatrix_;
         }

         auto ret = matrix{
            right_[0], right_[1], right_[2], Zero,
            up_[0], up_[1], up_[2], Zero,
            backward_[0], backward_[1], backward_[2], Zero,
            Zero, Zero, Zero, One };

         ret = ret * transl_matrix_; // translate into camera system origin first
                                     // then do the rotation

         viewmatrix_ = ret;
         bDirty_ = false;

         return ret;
      }

      constexpr void move_forward(NumericType distance) noexcept {
         if (bDirty_) {
            calc_rot_matrix();
         }
         // Y zeroed out because we don't want to move up or down
         position_ -= vec3<NumericType>{ backward_[0], 0.0f, backward_[2] } * distance;
         calc_transl_matrix();
         bDirty_ = true;
      }

      constexpr void move_right(NumericType distance) noexcept {
         if (bDirty_) {
            calc_rot_matrix();
         }
         // Y zeroed out because we don't want to move up or down
         position_ += vec3<NumericType>{ right_[0], 0.0f, right_[2] } * distance;
         calc_transl_matrix();
         bDirty_ = true;
      }

      constexpr void inc_yaw(NumericType angle) noexcept {
         yaw_ += angle;
         yaw_ = fmodf(yaw_, 2 * M_PI);
         calc_rot_matrix();
         bDirty_ = true;
      }

      constexpr void inc_pitch(NumericType angle) noexcept {
         pitch_ += angle;
         pitch_ = std::max(std::min(pitch_, std::numbers::pi_v<NumericType> / 4), -std::numbers::pi_v<NumericType> / 4);
         calc_rot_matrix();
         bDirty_ = true;
      }

   private:
      constexpr void calc_rot_matrix() noexcept {
         auto const s_pitch = sinf(-pitch_);
         auto const c_pitch = cosf(-pitch_);
         auto const s_yaw = sinf(-yaw_);
         auto const c_yaw = cosf(-yaw_);

         rot_matrix_ = matrix{
            c_yaw, s_yaw*s_pitch, s_yaw*c_pitch, Zero,
            Zero, c_pitch, -s_pitch, Zero,
            -s_yaw, c_yaw*s_pitch, c_yaw*c_pitch, Zero,
            Zero, Zero, Zero, One };

         backward_ = rot_matrix_ * vec4f{ 0.0f, 0.0f, 1.0f, 0.0f };
         up_ = rot_matrix_ * vec4f{ 0.0f, 1.0f, 0.0f, 0.0f };
         right_ = rot_matrix_ * vec4f{ 1.0f, 0.0f, 0.0f, 0.0f };
      }

      constexpr void calc_transl_matrix() noexcept {
         transl_matrix_ = matrix{
            One, Zero, Zero, -position_[0],
            Zero, One, Zero, -position_[1],
            Zero, Zero, One, -position_[2],
            Zero, Zero, Zero, One };
      }
   };

   using fps_camera = basic_fps_camera<float>;
}
