#pragma once
#include <iostream>
#include <optional>
#include <variant>

namespace sw::mngr::cmd {
enum class CmdType { UNDEF = 0, MOVE, M_ATCK, R_ATCK };

template <CmdType Type>
struct Description;

template <>
struct Description<CmdType::MOVE> {
  const int delta_x;
  const int delta_y;
};
using MoveDescription = Description<CmdType::MOVE> ;

template <>
struct Description<CmdType::M_ATCK> {
  const unsigned unit_id;
  const unsigned damage;
};
using MeleeAttackDescription = Description<CmdType::M_ATCK> ;

template <>
struct Description<CmdType::R_ATCK> {
  const unsigned unit_id;
  const unsigned range;
  const unsigned damage;
};
using RangeAttackDescription = Description<CmdType::R_ATCK> ;

struct CmdDescription {
    template<CmdType T>
    CmdDescription(unsigned id, Description<T> descr) : type_(T), id_(id), cmd_params_(descr){} ;

    const CmdType type_;
    const unsigned id_;
    const std::variant<
        MoveDescription, 
        MeleeAttackDescription,
        RangeAttackDescription
    > cmd_params_;

    template <CmdType Type>
    Description<Type> get_description() const {
        return std::get<Description<Type>>(cmd_params_);
    }

    bool operator==(const CmdDescription& rhs) const{
    return this->id_ == rhs.id_ &&
           this->type_ == rhs.type_;
    }

    friend std::ostream &operator<<(std::ostream &os, const CmdDescription &cmd) 
    {
    os << "ID: " << cmd.id_ << ", CmdType: ";
    switch (cmd.type_) {
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