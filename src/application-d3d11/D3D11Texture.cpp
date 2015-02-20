
namespace ml
{

void D3D11Texture::load(GraphicsDevice &g, const Bitmap &bmp)
{
    m_graphics = &g.castD3D11();
    release(g);
    
    g.castD3D11().registerAsset(this);
    m_bmp = bmp;

    reset(g);
}

void D3D11Texture::load(GraphicsDevice &g, const ColorImageR8G8B8A8 &image)
{
    Bitmap bmp(image.getHeight(), image.getWidth(), [&](size_t row, size_t col) {
        auto p = image(row, col);
        return RGBColor(p.x, p.y, p.z, p.w);
    });
    load(g, bmp);
}

void D3D11Texture::release(GraphicsDevice &g)
{
    SAFE_RELEASE(m_texture);
    SAFE_RELEASE(m_view);
}

void D3D11Texture::reset(GraphicsDevice &g)
{
    release(g);

    if (m_bmp.dimX() == 0 || m_bmp.dimY() == 0)
        return;

    auto &device = g.castD3D11().device();
    auto &context = g.castD3D11().context();

    D3D11_TEXTURE2D_DESC desc;
    desc.Width = (UINT)m_bmp.dimX();
    desc.Height = (UINT)m_bmp.dimY();
    desc.MipLevels = 0;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

    D3D_VALIDATE(device.CreateTexture2D(&desc, nullptr, &m_texture));
    D3D_VALIDATE(device.CreateShaderResourceView(m_texture, nullptr, &m_view));

    context.UpdateSubresource(m_texture, 0, nullptr, m_bmp.ptr(), (UINT)m_bmp.dimX() * sizeof(RGBColor), (UINT)m_bmp.dimX() * (UINT)m_bmp.dimY() * sizeof(RGBColor));

    context.GenerateMips(m_view);
}

void D3D11Texture::bind() const
{
    if (m_view == nullptr)
        return;
    m_graphics->context().PSSetShaderResources(0, 1, &m_view);
}

}