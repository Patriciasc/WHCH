<?xml version="1.0"?>
<xsl:stylesheet version="2.0"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:xs="http://www.w3.org/2001/XMLSchema">
<xsl:output method="text"/>

<xsl:template match="/">
<xsl:text>{| border="1" cellpadding="4" class="wikitable"
</xsl:text>
<xsl:text>! Date!! Client!! Duration!! Task!! Details</xsl:text>
<xsl:apply-templates select="year/week"/>
<xsl:text>
|}</xsl:text>
</xsl:template>

<xsl:template match="year/week">
<xsl:for-each select="day/task">
<xsl:sort select="@client"/>
<xsl:text>
|-
</xsl:text>
<xsl:text>|</xsl:text>
<xsl:value-of select="substring(@start,0,11)"/>
<xsl:text>||</xsl:text>
<xsl:value-of select="substring(@end,0,11)"/>
<xsl:text>||</xsl:text>
<xsl:value-of select="@client"/>
<xsl:text>||</xsl:text>
<xsl:call-template name="task_duration"/>
<xsl:text>||</xsl:text>
<xsl:value-of select="@name"/>
<xsl:text>||</xsl:text>
<xsl:value-of select="details"/>
</xsl:for-each>

<xsl:text>
|-
</xsl:text>
<xsl:text>| colspan="4" bgcolor="#dddddd" | Total hours (week </xsl:text>
<xsl:value-of select="@number"/>
<xsl:text>):</xsl:text>
<xsl:call-template name="total_duration"/>
</xsl:template>

<xsl:template name="task_duration">
  <xsl:variable name="duration" select="xs:dateTime(@end) - xs:dateTime(@start)"/>
  <xsl:value-of select="hours-from-duration($duration)"/><xsl:text>h</xsl:text>
  <xsl:value-of select="minutes-from-duration($duration)"/><xsl:text>m</xsl:text>
  <xsl:value-of select="seconds-from-duration($duration)"/><xsl:text>s</xsl:text>
</xsl:template>

<xsl:template name="total_duration">
  <xsl:variable name="duration" select="sum(for $x in //task return $x/xs:dateTime(@end) - $x/xs:dateTime(@start))"/>
  <xsl:value-of select="hours-from-duration($duration)"/><xsl:text>h</xsl:text>
  <xsl:value-of select="minutes-from-duration($duration)"/><xsl:text>m</xsl:text>
  <xsl:value-of select="seconds-from-duration($duration)"/><xsl:text>s</xsl:text>
</xsl:template>

</xsl:stylesheet>
