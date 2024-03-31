#pragma once
#include <iostream>
#include <optional>
#include <variant>

namespace sw::mngr::cmd {
enum class CmdType { UNDEF = 0, MOVE, M_ATCK, R_ATCK };

struct MoveDescription {
  const int delta_x;
  const int delta_y;
  const CmdType type_ = CmdType::MOVE;

  bool operator==(const MoveDescription& rhs) {
    return this->delta_x == rhs.delta_x && this->delta_y == rhs.delta_y && this->type_ == rhs.type_;
}

};

struct MeleeAttackDescription {
  const unsigned unit_id;
  const unsigned damage;
  const CmdType type_ = CmdType::M_ATCK;
};

struct RangeAttackDescription {
  const unsigned unit_id;
  const unsigned range;
  const unsigned damage;
  const CmdType type_ = CmdType::R_ATCK;
};

struct CmdDescription {
    template<class TDescr>
    CmdDescription(unsigned id, TDescr descr) : type_(descr.type_), id_(id), cmd_params_(descr){} ;

    const CmdType type_;
    const unsigned id_;
    //todo: CRTP
    const std::variant<
        MoveDescription, 
        MeleeAttackDescription,
        RangeAttackDescription
    > cmd_params_;

    template<class T>
    T get_description() const
    {
        return std::get<T>(cmd_params_);
    }

    bool operator==(const CmdDescription& rhs) {
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
            const MoveDescription& move_descr = cmd.get_description<MoveDescription>();
            os << ", Move: (" << move_descr.delta_x << ", " << move_descr.delta_y << ")";
            break;
        }
        case CmdType::M_ATCK: {
            os << "MELEE ATTACK";
            const MeleeAttackDescription& m_atck_descr = cmd.get_description<MeleeAttackDescription>();
            os << ", Melee Attack: (Unit ID: " << m_atck_descr.unit_id << ", Damage: " << m_atck_descr.damage << ")";
            break;
        }
        case CmdType::R_ATCK: {
            os << "RANGE ATTACK";
            const RangeAttackDescription& r_atck_descr = cmd.get_description<RangeAttackDescription>();
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