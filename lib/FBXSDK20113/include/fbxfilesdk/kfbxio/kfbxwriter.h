/*!  \file kfbxwriter.h
 */

#ifndef FBXFILESDK_KFBXIO_KFBXWRITER_H
#define FBXFILESDK_KFBXIO_KFBXWRITER_H

/**************************************************************************************

 Copyright (C) 2001 - 2010 Autodesk, Inc. and/or its licensors.
 All Rights Reserved.

 The coded instructions, statements, computer programs, and/or related material 
 (collectively the "Data") in these files contain unpublished information 
 proprietary to Autodesk, Inc. and/or its licensors, which is protected by 
 Canada and United States of America federal copyright law and by international 
 treaties. 
 
 The Data may not be disclosed or distributed to third parties, in whole or in
 part, without the prior written consent of Autodesk, Inc. ("Autodesk").

 THE DATA IS PROVIDED "AS IS" AND WITHOUT WARRANTY.
 ALL WARRANTIES ARE EXPRESSLY EXCLUDED AND DISCLAIMED. AUTODESK MAKES NO
 WARRANTY OF ANY KIND WITH RESPECT TO THE DATA, EXPRESS, IMPLIED OR ARISING
 BY CUSTOM OR TRADE USAGE, AND DISCLAIMS ANY IMPLIED WARRANTIES OF TITLE, 
 NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE OR USE. 
 WITHOUT LIMITING THE FOREGOING, AUTODESK DOES NOT WARRANT THAT THE OPERATION
 OF THE DATA WILL BE UNINTERRUPTED OR ERROR FREE. 
 
 IN NO EVENT SHALL AUTODESK, ITS AFFILIATES, PARENT COMPANIES, LICENSORS
 OR SUPPLIERS ("AUTODESK GROUP") BE LIABLE FOR ANY LOSSES, DAMAGES OR EXPENSES
 OF ANY KIND (INCLUDING WITHOUT LIMITATION PUNITIVE OR MULTIPLE DAMAGES OR OTHER
 SPECIAL, DIRECT, INDIRECT, EXEMPLARY, INCIDENTAL, LOSS OF PROFITS, REVENUE
 OR DATA, COST OF COVER OR CONSEQUENTIAL LOSSES OR DAMAGES OF ANY KIND),
 HOWEVER CAUSED, AND REGARDLESS OF THE THEORY OF LIABILITY, WHETHER DERIVED
 FROM CONTRACT, TORT (INCLUDING, BUT NOT LIMITED TO, NEGLIGENCE), OR OTHERWISE,
 ARISING OUT OF OR RELATING TO THE DATA OR ITS USE OR ANY OTHER PERFORMANCE,
 WHETHER OR NOT AUTODESK HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH LOSS
 OR DAMAGE. 

**************************************************************************************/
#include <fbxfilesdk/fbxfilesdk_def.h>

#include <fbxfilesdk/components/kbaselib/klib/kerror.h>
#include <fbxfilesdk/components/kbaselib/klib/kstring.h>
#include <fbxfilesdk/components/kbaselib/klib/karrayul.h>

#include <fbxfilesdk/kfbxplugins/kfbxrenamingstrategy.h>

#include <fbxfilesdk/fbxfilesdk_nsbegin.h>

class KFbxNode;
class KFbxDocument;
class KFbxScene;
class KFbxSdkManager;
class KFbxExporter;
class KFbxIOSettings;
class KFbxObject;
class KFbx;
class KFbxProgress;

#define IOSP GetIOSettings()

 /** Base class of other writers used internally.
   * This class provides the interfaces for writing files. 
   *
   * The role of the writer is to effectively "write" specific file data
   * vs the role of the exporter is to select a specific writer
   * and launch the writing of a file through that writer.
   * \see KFbxExporter 
   *
   * ex:
   * - KFbxWriterFbx  can write FBX 5 format files
   * - KFbxWriterFbx6 can write FBX 6 format files
   * - KFbxWriterFbx7 can write FBX 7 format files
   * - KFbxWriterCollada can write Collada files
   * - KFbxWriterDxf can write Dxf files
   * - ... etc.
   *
   * A SDK user should - normally - not use this class,
   * except if a custom writer must be created for plug-in extension,
   * then KFbxWriter must be the base class for 
   * the new custom writer in that particular situation.
   * \nosubgrouping
   */
class KFBX_DLL KFbxWriter
{
public:
	/** Constructor
      *	\param pManager        The KFbxSdkManager Object
      * \param pID             Id for current writer
      */
    KFbxWriter(KFbxSdkManager& pManager, int pID);
	/** Destructor
      */
    virtual ~KFbxWriter();

   	/** \enum EError  Error identifiers.
              * - \e eFILE_CORRUPTED
              * - \e eFILE_NOT_OPENED
              * - \e eFILE_NOT_CREATED
              * - \e eOUT_OF_DISK_SPACE
              * - \e eINVALID_DOCUMENT_HANDLE
              * - \e eDOCUMENT_NOT_SUPPORTED
			  * - \e eUNIDENTIFIED_ERROR
			  * - \e eEMBEDDED_OUT_OF_SPACE
              * - \e eERROR_COUNT			  
              */
    typedef enum
    {
        eFILE_CORRUPTED,
        eFILE_NOT_OPENED,
        eFILE_NOT_CREATED,
        eOUT_OF_DISK_SPACE,
        eINVALID_DOCUMENT_HANDLE,
        eDOCUMENT_NOT_SUPPORTED,
        eUNIDENTIFIED_ERROR,
        eEMBEDDED_OUT_OF_SPACE,
        eERROR_COUNT
    } EError;

	/** \enum KInfoRequest
			  * - \e eInfoExtension		    // to get the file ext for a writer ex: "FBX"
			  * - \e eInfoDescriptions      // to get the file description for a writer ex:"Autodesk FBX (*.fbx)"
			  * - \e eInfoVersions          // to get the file version for a writer ex: 7100
			  * - \e eInfoCompatibleDesc    // to get the file compatible description for a writer.
			  * - \e eInfoUILabel           // to get the file UI label to show for a writer ex: file labels shown in "Open file dialog"
			  * - \e eReserved1
			  * \remarks Used internally to get writer file information.
			  */
    enum KInfoRequest {
        eInfoExtension,      
        eInfoDescriptions,   
        eInfoVersions,       
		eInfoCompatibleDesc, 
		eInfoUILabel,        
        eReserved1 = 0xFBFB,
    };
	
	//! Helper typedef for passing KFbxWriter creator function as argument (used internally)
    typedef KFbxWriter*				(*CreateFuncType)(KFbxSdkManager& pManager, KFbxExporter& pExporter, int pSubID, int pPluginID);

	//! Helper typedef for passing KFbxIOSettings creator function as argument (used internally)
    typedef void					(*IOSettingsFillerFuncType)(KFbxIOSettings& pIOS);

	//! Helper typedef for passing KInfoRequest function as argument (used internally)
    typedef void*					(*GetInfoFuncType)(KInfoRequest pRequest, int pWriterTypeId);

    /** Creates a new file.
      * \param pFileName The name of the newly created file.
      */
    virtual bool					FileCreate(char* pFileName) = 0;
	
	/** Closes the file.
	*/
    virtual bool					FileClose() = 0;
	
	/**  Test if the file is open.
	*/
    virtual bool					IsFileOpen() = 0;

    /**  Setup write options.
	*/
    virtual void		            GetWriteOptions() = 0;

	/** Writes content to the specified file with given stream options
      *	\param pDocument        KFbxDocument to write file data to.
	  */
    virtual bool					Write(KFbxDocument* pDocument) = 0;

	/** Pre-processes the scene.
      * \param pScene The scene needs to be pre-processed.
      */
    virtual bool					PreprocessScene(KFbxScene &pScene) = 0;

	/** Post-processes the scene.
      * \param pScene The scene needs to be post-processed.
      */
    virtual bool					PostprocessScene(KFbxScene &pScene) = 0;

	/** Writes extension plug-ins name, version and parameters, so that we can remember if a plug-in was used during export.
	  * This is especially useful for extension plug-ins that modify the scene and also to warn users during import if an
	  * extension plug-in was used that could be missing.
      * \param pParams The parameters of the extension plug-in. The properties of the objects are used
	  * as the parameters of the extension plug-in.
	  * \remark This function has no implementation in this class. Only sub-class should implement it as needed. For example,
	  * FBX 6 and FBX 7 does implement it.
      */
	virtual void					PluginWriteParameters(KFbxObject& pParams);

	/** Finds the selected root node in the specified scene.
      * \param pScene	The scene in which the selected root node is found.
	  *	\return			The located root node.\c NULL if the selected root node cannot be found.
      */
    virtual KFbxNode*				FindRootNode(KFbxScene& pScene);

	/** Checks if there are spaces in the names of specified node (and its children nodes),
	  * and writes the returned node's name in the specified string list.
      * \param pNode	Specifies the node to check.
	  * \param pNodeNameList Specifies the string list where the node name that has spaces in it is recorded.
	  *	\return \c true If there are no spaces in the name of specified node (and its children nodes), 
	  * \c false  If spaces are found.
      */
    virtual bool					CheckSpaceInNodeNameRecursive(KFbxNode* pNode, KString& pNodeNameList);

	/** Sets the file export version as specified.
      * \param pVersion The specified file export version.
      */
    bool							SetFileExportVersion(KString pVersion);
 
	/** Sets the renaming mode as specified.
      * \param pRenamingMode The specified renaming mode.
      */
    void							SetRenamingMode(KFbxSceneRenamer::ERenamingMode pRenamingMode){mRenamingMode = pRenamingMode;}

	/** Sets the resampling rate as specified.
      * \param pResamplingRate The specified resampling rate.
      */
    inline void						SetResamplingRate(double pResamplingRate){mResamplingRate = pResamplingRate;}

	/** Test if file format is an internal plug-in .
	  * A non genuine plug-in is a plug-in made by someone external to Autodesk FBX SDK group.
      *	\return \c true If the file format is an internal plug-in ,\c false Otherwise .
      */
	bool							IsGenuine();

	/** Returns error object(s).
      * \return References to the error object(s).
      */
    KError&							GetError();

	/** Returns the ID of the last error that occurred when the file was written.
      * \return The last error ID.
      */
    EError							GetLastErrorID() const;

	/** Returns the error string that describes the last error that occurred when the file was written.
      * \return A textual description of the last error.
      */
    const char*						GetLastErrorString() const;

	/** Returns a warning message that describes what occurred when the file was written.
      *	\param pMessage The returned warning message.
      */
    void							GetMessage(KString &pMessage) const;

	/** Returns a warning message that describes what occurred when the file was written.
      *	\return The returned warning message.
      */
    KString&						GetMessage();

	/** Empties the message string.
      */
    void							ClearMessage();

    /** Access to a IOSettings object.
      * \return The pointer to IOSettings or \c NULL \c if the object
      * has not been allocated.
    */
	virtual KFbxIOSettings * GetIOSettings();

	/** Set the IOSettings pointer to be used for this writer instance.
	  * \param pIOSettings  
	  */
	virtual void SetIOSettings(KFbxIOSettings * pIOSettings);

    /** Pass a progress handler to the writer
      * \param pProgress     KFbxProgress to store the progress information.
      */
    virtual void SetProgressHandler(KFbxProgress *pProgress){}

protected:
	void							PluginsWriteBegin(KFbxScene& pScene);
	void							PluginsWrite(KFbx& pFbx, bool pWriteObjectId);
	void							PluginsWriteEnd(KFbxScene& pScene);

    KFbxWriter&						operator=(KFbxWriter const&) { return *this; }

    KFbxSdkManager& 				mManager;
    KString							mFileVersion;
    double							mResamplingRate;
    KFbxSceneRenamer::ERenamingMode	mRenamingMode;

private:
    KError							mError;
    KString         				mMessage;
	int								mInternalID;
	KFbxIOSettings *                mIOSettings;

	friend struct KFbxWriterFbx7Impl;
};

//! Helper to access the IOSetting object pointer as a ref ex: IOS_REF.GetBoolProp( ... );
#define IOS_REF (*GetIOSettings())

#include <fbxfilesdk/fbxfilesdk_nsend.h>

#endif // FBXFILESDK_KFBXIO_KFBXWRITER_H

