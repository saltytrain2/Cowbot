#include <wx/wx.h>

class ChessBoardPiece : public wxObject
{
public:
    ChessBoardPiece(const wxImage& iamge = wxImage(), bool show = true);
    void setShow(bool show);
    bool getShow() const;
    void setImage(const wxImage& image);
    wxImage getImage() const;
    void scale(int width, int height);

private:
    wxImage mImage;
    bool mShow;
};


ChessBoardPiece::ChessBoardPiece(const wxImage& image, bool show)
    : mImage(image),
      mShow(show)
{}

void ChessBoardPiece::setShow(bool show)
{
    mShow = show;
}

bool ChessBoardPiece::getShow() const
{
    return mShow;
}

void ChessBoardPiece::setImage(const wxImage& image)
{
    mImage = image;
}

wxImage ChessBoardPiece::getImage() const
{
    return mImage;
}

void ChessBoardPiece::scale(int width, int height)
{
    mImage.Rescale(width, height, wxIMAGE_QUALITY_HIGH);
}