<?xml version="1.0"?>
<xsl:stylesheet version="2.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:xs="http://www.w3.org/2001/XMLSchema">

<xsl:output method="text"/>

<xsl:template match="/">
<xsl:text>{| border="1" cellpadding="4" class="wikitable"
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
<!-- Format this in a more human readable format. -->
<xsl:variable name="duration" select="xs:dateTime(@end) - xs:dateTime(@start)"/>
<xsl:value-of select="$duration"/>
<xsl:text>||</xsl:text>
<xsl:value-of select="@name"/>
<xsl:text>||</xsl:text>
<xsl:value-of select="details"/>
</xsl:for-each>
<xsl:text>
|}</xsl:text>
</xsl:template>
</xsl:stylesheet>

