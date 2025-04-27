//
// Created by Rakesh on 04/06/2023.
//

#include "pgtypes.hpp"

#include <boost/algorithm/string/replace.hpp>

std::string_view spt::tsdb::columnType( std::uint32_t type )
{
  switch ( type )
  {
  case 16: return { "Byte" };
  case 17: return { "Bytea" };
  case 18: return { "Char" };
  case 19: return { "Name" };
  case 20: return { "Int8" };
  case 21: return { "Int2" };
  case 22: return { "Int2vector" };
  case 23: return { "Int4" };
  case 24: return { "Regproc" };
  case 25: return { "Text" };
  case 26: return { "Oid" };
  case 27: return { "Tid" };
  case 28: return { "Xid" };
  case 29: return { "Cid" };
  case 30: return { "Oidvector" };
  case 71: return {"Pg_type" };
  case 75: return {"Pg_attribute" };
  case 81: return {"Pg_proc" };
  case 83: return {"pg_class" };
  case 114: return {"Json" };
  case 142: return {"Xml" };
  case 143: return {"Xml1" };
  case 194: return {"Pg_node_tree" };
  case 199: return {"Json1" };
  case 210: return {"Smgr" };
  case 600: return {"Point" };
  case 601: return {"Lseg" };
  case 602: return {"Path" };
  case 603: return {"Box" };
  case 604: return {"Polygon" };
  case 628: return {"Line" };
  case 629: return {"Line1" };
  case 650: return {"Cidr" };
  case 651: return {"cidr1" };
  case 700: return {"Float4" };
  case 701: return {"Float8" };
  case 702: return {"Abstime" };
  case 703: return {"Reltime" };
  case 704: return {"Tinterval" };
  case 705: return {"Unknown" };
  case 718: return {"Circle" };
  case 719: return {"Circle1" };
  case 790: return {"Money" };
  case 791: return {"Money1" };
  case 829: return {"Macaddr" };
  case 869: return {"Inet" };
  case 1000: return {"Bool1" };
  case 1001: return {"Bytea1" };
  case 1002: return {"Char1" };
  case 1003: return {"Name1" };
  case 1005: return {"Int21" };
  case 1006: return {"Int2vector1" };
  case 1007: return {"Int41" };
  case 1008: return {"Regproc1" };
  case 1009: return {"Text1" };
  case 1010: return {"Tid1" };
  case 1011: return {"Xid1" };
  case 1012: return {"Cid1" };
  case 1013: return {"Oidvector1" };
  case 1014: return {"Bpchar1" };
  case 1015: return {"Varchar1" };
  case 1016: return {"Int81" };
  case 1017: return {"Point1" };
  case 1018: return {"Lseg1" };
  case 1019: return {"Path1" };
  case 1020: return {"Box1" };
  case 1021: return {"Float41" };
  case 1022: return {"Float81" };
  case 1023: return {"Abstime1" };
  case 1024: return {"Reltime1" };
  case 1025: return {"Tinterval1" };
  case 1027: return {"Polygon1" };
  case 1028: return {"Oid1" };
  case 1033: return {"Aclitem" };
  case 1034: return {"Aclitem1" };
  case 1040: return {"Macaddr1" };
  case 1041: return {"Inet1" };
  case 1042: return {"Bpchar" };
  case 1043: return {"Varchar" };
  case 1082: return {"Date" };
  case 1083: return {"Time" };
  case 1114: return {"Timestamp" };
  case 1115: return {"Timestamp1" };
  case 1182: return {"Date1" };
  case 1183: return {"Time1" };
  case 1184: return {"Timestamptz" };
  case 1185: return {"Timestamptz1" };
  case 1186: return {"Interval" };
  case 1187: return {"Interval1" };
  case 1231: return {"Numeric1" };
  case 1248: return {"Pg_database" };
  case 1263: return {"Cstring1" };
  case 1266: return {"Timetz" };
  case 1270: return {"Timetz1" };
  case 1560: return {"Bit" };
  case 1561: return {"Bit1" };
  case 1562: return {"Varbit" };
  case 1563: return {"Varbit1" };
  case 1700: return {"Numeric" };
  case 1790: return {"Refcursor" };
  case 2201: return {"Refcursor1" };
  case 2202: return {"Regprocedure" };
  case 2203: return {"Regoper" };
  case 2204: return {"Regoperator" };
  case 2205: return {"Regclass" };
  case 2206: return {"Regtype" };
  case 2207: return {"Regprocedure1" };
  case 2208: return {"Regoper1" };
  case 2209: return {"Regoperator1" };
  case 2210: return {"Regclass1" };
  case 2211: return {"Regtype1" };
  case 2249: return {"Record" };
  case 2275: return {"Cstring" };
  case 2276: return {"Any" };
  case 2277: return {"Anyarray" };
  case 2278: return {"Void" };
  case 2279: return {"Trigger" };
  case 2280: return {"Language_handler" };
  case 2281: return {"Internal" };
  case 2282: return {"Opaque" };
  case 2283: return {"Anyelement" };
  case 2287: return {"Record1" };
  case 2776: return {"Anynonarray" };
  case 2842: return {"Pg_authid" };
  case 2843: return {"Pg_auth_members" };
  case 2949: return {"Txid_snapshot1" };
  case 2950: return {"Uuid" };
  case 2951: return {"Uuid1" };
  case 2970: return {"Txid_snapshot" };
  case 3115: return {"Fdw_handler" };
  case 3500: return {"Anyenum" };
  case 3614: return {"Tsvector" };
  case 3615: return {"Tsquery" };
  case 3642: return {"Gtsvector" };
  case 3643: return {"Tsvector1" };
  case 3644: return {"Gtsvector1" };
  case 3645: return {"Tsquery1" };
  case 3734: return {"Regconfig" };
  case 3735: return {"Regconfig1" };
  case 3769: return {"Regdictionary" };
  case 3770: return {"Regdictionary1" };
  case 3831: return {"Anyrange" };
  case 3838: return {"Event_trigger" };
  case 3904: return {"Int4range" };
  case 3905: return {"Int4range1" };
  case 3906: return {"Numrange" };
  case 3907: return {"Numrange1" };
  case 3908: return {"Tsrange" };
  case 3909: return {"Tsrange1" };
  case 3910: return {"Tstzrange" };
  case 3911: return {"Tstzrange1" };
  case 3912: return {"Daterange" };
  case 3913: return {"Daterange1" };
  case 3926: return {"Int8range" };
  case 3927: return {"Int8range1" };
  }

  return { "Unmapped" };
}

bool spt::tsdb::isTimestamp( const pqxx::field& field )
{
  switch ( ColumnType( field.type() ) )
  {
  case ColumnType::Abstime:
  case ColumnType::Date:
  case ColumnType::Date1:
  case ColumnType::Timestamp:
  case ColumnType::Timestamp1:
  case ColumnType::Timestamptz:
  case ColumnType::Timestamptz1: return true;
  default: return false;
  }
}

bool spt::tsdb::isString( const pqxx::field& field )
{
  switch ( ColumnType( field.type() ) )
  {
  case ColumnType::Varchar:
  case ColumnType::Varchar1:
  case ColumnType::Char:
  case ColumnType::Char1:
  case ColumnType::Text:
  case ColumnType::Text1: return true;
  default: return false;
  }
}

bool spt::tsdb::isNumber( const pqxx::field &field )
{
  switch ( ColumnType( field.type() ) )
  {
  case ColumnType::Int2:
  case ColumnType::Int21:
  case ColumnType::Int4:
  case ColumnType::Int41:
  case ColumnType::Int8:
  case ColumnType::Int81:
  case ColumnType::Float4:
  case ColumnType::Float41:
  case ColumnType::Float8:
  case ColumnType::Float81:
  case ColumnType::Numeric:
  case ColumnType::Numeric1:
    return true;
  default: return false;
  }
}

std::optional<spt::util::DateTime> spt::tsdb::timestamp( const pqxx::field& field )
{
  switch ( ColumnType( field.type() ) )
  {
  case ColumnType::Abstime:
  case ColumnType::Date:
  case ColumnType::Date1:
  case ColumnType::Timestamp:
  case ColumnType::Timestamp1:
  case ColumnType::Timestamptz:
  case ColumnType::Timestamptz1:
  {
    auto value = std::string{ field.view() };
    boost::algorithm::replace_first( value, " ", "T" );
    if ( !value.ends_with( 'Z' ) && value.find( '+' ) == std::string::npos )
    {
      if ( value.size() > 10 && value.find( '-', 10 ) == std::string::npos )
      value.append( "Z" );
    }

    if ( auto var = spt::util::parseISO8601( value ); std::holds_alternative<util::DateTime>( var ) ) return std::get<util::DateTime>( var );
    break;
  }
  default: return std::nullopt;
  }

  return std::nullopt;
}

std::optional<std::string> spt::tsdb::string( const pqxx::field& field )
{
  switch ( ColumnType( field.type() ) )
  {
  case ColumnType::Varchar:
  case ColumnType::Varchar1:
  case ColumnType::Char:
  case ColumnType::Char1:
  case ColumnType::Text:
  case ColumnType::Text1: return field.as<std::string>();
  default: return std::nullopt;
  }
}

std::optional<double> spt::tsdb::number( const pqxx::field& field )
{
  switch ( ColumnType( field.type() ) )
  {
  case ColumnType::Int2:
  case ColumnType::Int21:
  case ColumnType::Int4:
  case ColumnType::Int41:
  case ColumnType::Int8:
  case ColumnType::Int81:
  case ColumnType::Float4:
  case ColumnType::Float41:
  case ColumnType::Float8:
  case ColumnType::Float81:
  case ColumnType::Numeric:
  case ColumnType::Numeric1:
    return field.as<double>();
  default: return std::nullopt;
  }
}
