//////////////////////////////////////////////////////////////////////////////
// Program Name: videoMetadataInfoList.h
// Created     : Apr. 21, 2011
//
// Copyright (c) 2011 Robert McNamara <rmcnamara@mythtv.org>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or at your option any later version of the LGPL.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library.  If not, see <http://www.gnu.org/licenses/>.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef VIDEOMETADATAINFOLIST_H_
#define VIDEOMETADATAINFOLIST_H_

#include <QVariantList>

#include "serviceexp.h"
#include "datacontracthelper.h"

#include "videoMetadataInfo.h"

namespace DTC
{

class SERVICE_PUBLIC VideoMetadataInfoList : public QObject
{
    Q_OBJECT
    Q_CLASSINFO( "version", "1.01" );

    // We need to know the type that will ultimately be contained in
    // any QVariantList or QVariantMap.  We do his by specifying
    // A Q_CLASSINFO entry with "<PropName>_type" as the key
    // and the type name as the value

    Q_CLASSINFO( "VideoMetadataInfos_type", "DTC::VideoMetadataInfo");

    Q_PROPERTY( int          StartIndex     READ StartIndex      WRITE setStartIndex     )
    Q_PROPERTY( int          Count          READ Count           WRITE setCount          )
    Q_PROPERTY( int          CurrentPage    READ CurrentPage     WRITE setCurrentPage    )
    Q_PROPERTY( int          TotalPages     READ TotalPages      WRITE setTotalPages     )
    Q_PROPERTY( int          TotalAvailable READ TotalAvailable  WRITE setTotalAvailable )
    Q_PROPERTY( QDateTime    AsOf           READ AsOf            WRITE setAsOf           )
    Q_PROPERTY( QString      Version        READ Version         WRITE setVersion        )
    Q_PROPERTY( QString      ProtoVer       READ ProtoVer        WRITE setProtoVer       )

    Q_PROPERTY( QVariantList VideoMetadataInfos READ VideoMetadataInfos DESIGNABLE true )

    PROPERTYIMP       ( int         , StartIndex      )
    PROPERTYIMP       ( int         , Count           )
    PROPERTYIMP       ( int         , CurrentPage     )
    PROPERTYIMP       ( int         , TotalPages      )
    PROPERTYIMP       ( int         , TotalAvailable  )
    PROPERTYIMP       ( QDateTime   , AsOf            )
    PROPERTYIMP       ( QString     , Version         )
    PROPERTYIMP       ( QString     , ProtoVer        )

    PROPERTYIMP_RO_REF( QVariantList, VideoMetadataInfos )

    public:

        static void InitializeCustomTypes()
        {
            qRegisterMetaType< VideoMetadataInfoList  >();
            qRegisterMetaType< VideoMetadataInfoList* >();

            VideoMetadataInfo::InitializeCustomTypes();
        }

    public:

        VideoMetadataInfoList(QObject *parent = 0)
            : QObject( parent ),
              m_StartIndex    ( 0      ),
              m_Count         ( 0      ),
              m_CurrentPage   ( 0      ),
              m_TotalPages    ( 0      ),
              m_TotalAvailable( 0      )
        {
        }

        VideoMetadataInfoList( const VideoMetadataInfoList &src )
        {
            Copy( src );
        }

        void Copy( const VideoMetadataInfoList &src )
        {
            m_StartIndex    = src.m_StartIndex     ;
            m_Count         = src.m_Count          ;
            m_TotalAvailable= src.m_TotalAvailable ;
            m_AsOf          = src.m_AsOf           ;
            m_Version       = src.m_Version        ;
            m_ProtoVer      = src.m_ProtoVer       ;

            CopyListContents< VideoMetadataInfo >( this, m_VideoMetadataInfos, src.m_VideoMetadataInfos );
        }

        VideoMetadataInfo *AddNewVideoMetadataInfo()
        {
            // We must make sure the object added to the QVariantList has
            // a parent of 'this'

            VideoMetadataInfo *pObject = new VideoMetadataInfo( this );
            m_VideoMetadataInfos.append( QVariant::fromValue<QObject *>( pObject ));

            return pObject;
        }

};

} // namespace DTC

Q_DECLARE_METATYPE( DTC::VideoMetadataInfoList  )
Q_DECLARE_METATYPE( DTC::VideoMetadataInfoList* )

#endif
