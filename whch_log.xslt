<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output method="text"/>

<xsl:template match="/" indent="yes" >
    <xsl:text>
    {| border="1" cellpadding="4" class="wikitable"
    </xsl:text>
    <xsl:text>! Client!! Duration!! Task!! Details</xsl:text>
    <xsl:for-each select="year/day/task">
    <xsl:sort select="@client"/>
    <xsl:text>
    |-
    </xsl:text>
      <xsl:text>|</xsl:text>
      <xsl:value-of select="@client"/>
      <xsl:text>||</xsl:text>
      <xsl:value-of select="@client"/>
      <xsl:text>||</xsl:text>
      <xsl:value-of select="@name"/>
      <xsl:text>||</xsl:text>
      <xsl:value-of select="details"/>
    </xsl:for-each>
    <xsl:text>
    |}
    </xsl:text>
</xsl:template>
</xsl:stylesheet>

