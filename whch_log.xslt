<?xml version="1.0" encoding="ISO-8859-1"?>

<xsl:stylesheet version="1.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/">
  <xsl:text>{| border="1"</xsl:text><br/>
  <xsl:text>!Client||Duration||Task</xsl:text><br/>
  <xsl:for-each select="year/day/task">
    <xsl:sort select="@client"/>
      <xsl:text>|-</xsl:text><br/>
      <xsl:text>|</xsl:text>
      <xsl:value-of select="@client"/>
      <xsl:text>||</xsl:text>
      <xsl:value-of select="@end"/>
      <xsl:text>||</xsl:text>
      <xsl:value-of select="@name"/><br/>
  </xsl:for-each>
  <xsl:text>|}</xsl:text>
</xsl:template>
</xsl:stylesheet>

