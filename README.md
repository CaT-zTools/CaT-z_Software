# CaT-z_Software
CaT-z: Classification and Tracking in depth (z) - Software for the acquisition, annotation, and automatic behavioral profile characterization of laboratory rodents' RGB-D videos
[UPDATED VERSION - jun 2021]

CaT-z is composed of three modules. 
To run the already-compiled version, download the .zip file using the "Release" tab (https://github.com/CaT-zTools/CaT-z_Software/releases/tag/v1.0) and run the following batch/msi files:

RGB-D data Acquisition GUI (with the Microsoft Kinect v2 camera or the Intel Real Sense camera):
> CaT_z_dataAcquisitionGUI_RealSense.msi 
> CaT_z_dataAcquisitionGUI_Kinect.bat 

RGB-D data Visualization and Annotation:
> CaT_z_VisualizationAnnotationGUI.bat

3D Tracking and Classification GUI:
> CaT_z_TrackingClassificationGUI.bat

To run the Data Acquisition module with the Microsoft Kinect v2 camera, you need first to install Microsoft Kinect SDK 2.0. Please use the following link to download the installation file:
https://www.microsoft.com/en-us/download/details.aspx?id=44561

Tutorials (User guide and video) are also available.

The non-compiled code is available in the "src" folder.

If you use this software, please acknowledge our work published in: 
“Gerós, A., Magalhães, A. &  Aguiar, P. Improved 3D tracking and automated classification of rodents' behavioral activity using depth-sensing cameras. Behav Res (2020), doi: 10.3758/s13428-020-01381-9”

Thank you for your interest in the CaT-z software.
For questions/comments, please send us an email to:
anafgeros@ineb.up.pt or pauloaguiar@ineb.up.pt

THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
