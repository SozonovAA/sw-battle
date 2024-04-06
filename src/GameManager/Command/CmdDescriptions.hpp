#pragma once
#include <any>
#include <ostream>

namespace sw::mngr::cmd {
enum class CmdType { UNDEF = 0, MOVE, M_ATCK, R_ATCK };

template <CmdType Type>
struct Description;

template <>
struct Description<CmdType::MOVE> {
  const int delta_x;
  const int delta_y;
  bool operator==(const Description<CmdType::MOVE>& rhs) const {
    return delta_x == rhs.delta_x && delta_y == rhs.delta_y;
  }
};
using MoveDescription = Description<CmdType::MOVE> ;

template <>
struct Description<CmdType::M_ATCK> {
  const unsigned unit_id;
  const unsigned damage;
  bool operator==(const Description<CmdType::M_ATCK>& rhs) const {
      return unit_id == rhs.unit_id && damage == rhs.damage;
  }
};
using MeleeAttackDescription = Description<CmdType::M_ATCK> ;

template <>
struct Description<CmdType::R_ATCK> {
  const unsigned unit_id;
  const unsigned range;
  const unsigned damage;

  bool operator==(const Description<CmdType::R_ATCK>& rhs) const {
      return unit_id == rhs.unit_id && range == rhs.range && damage == rhs.damage;
  }
};
using RangeAttackDescription = Description<CmdType::R_ATCK>;

struct CmdDescription {
    template<CmdType T>
    CmdDescription(unsigned id, Description<T> descr) : _type(T), _id(id), _cmd_params(descr){} ;

    const CmdType _type;
    const unsigned _id;
    const std::any _cmd_params;

    template <CmdType Type>
    [[nodiscard]] Description<Type> get_description() const {
      return std::any_cast<Description<Type>>(_cmd_params);
    }
    
    bool operator==(const CmdDescription& rhs) const{
    return this->_id == rhs._id &&
           this->_type == rhs._type;
    }

    friend std::ostream &operator<<(std::ostream &os, const CmdDescription &cmd)
    {
      os << "ID: " << cmd._id << ", CmdType: ";
      switch (cmd._type) {
          case CmdType::UNDEF:
              os << "UNDEFINED";
              break;
          case CmdType::MOVE: {
              os << "MOVE";
              const MoveDescription& move_descr = cmd.get_description<CmdType::MOVE>();
              os << ", Move: (" << move_descr.delta_x << ", " << move_descr.delta_y << ")";
              break;
          }
          case CmdType::M_ATCK: {
              os << "MELEE ATTACK";
              const MeleeAttackDescription& m_atck_descr = cmd.get_description<CmdType::M_ATCK>();
              os << ", Melee Attack: (Unit ID: " << m_atck_descr.unit_id << ", Damage: " << m_atck_descr.damage << ")";
              break;
          }
          case CmdType::R_ATCK: {
              os << "RANGE ATTACK";
              const RangeAttackDescription& r_atck_descr = cmd.get_description<CmdType::R_ATCK>();
              os << ", Range Attack: (Unit ID: " << r_atck_descr.unit_id << ", Range: " << r_atck_descr.range 
                << ", Damage: " << r_atck_descr.damage << ")";
              break;
          }
          default:
              os << "UNKNOWN";
              break;
      }
      return os;
    }
};

} // namespace sw::mngr::cmd